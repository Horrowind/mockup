#include <cstdint>
#include <cstdio>
#include <cstring>

#include <iostream>

#include <QDockWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QTimer>

#include "debug.hpp"
#include "cpu.hpp"
#include "level.hpp"
#include "main_window.hpp"



namespace Mockup {
    

    MainWindow::MainWindow(int level) : m_level("smw.sfc", level) {
        m_currentLevel = level;
        setDocumentMode(true);
        grabKeyboard();
        
        setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);

        m_dockDebug = new QDockWidget("Debug", this);
        m_textDebug = new QTextEdit(m_dockDebug);
        m_textDebug->setReadOnly(true);
        m_dockDebug->setWidget(m_textDebug);

        initializeDebug(m_textDebug);

        m_viewMap8 = new QGraphicsView(&m_sceneMap8);
        m_viewMap8->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_viewMap8->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_viewMap8->setMinimumSize(128, 256);
        m_viewMap8->setMaximumSize(128, 256);
      
        m_dockMap8 = new QDockWidget("Map 8", this);
        m_dockMap8->setWidget(m_viewMap8);
        
        m_viewMap16 = new QGraphicsView(&m_sceneMap16);
        m_viewMap16->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_viewMap16->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_viewMap16->setMinimumSize(256, 256);
        m_viewMap16->setMaximumSize(256, 256);
      
        m_dockMap16 = new QDockWidget("Map 16", this);
        m_dockMap16->setWidget(m_viewMap16);
        
        addDockWidget(Qt::BottomDockWidgetArea, m_dockMap8);
        addDockWidget(Qt::BottomDockWidgetArea, m_dockMap16);
        addDockWidget(Qt::BottomDockWidgetArea, m_dockDebug);
        m_view = new QGraphicsView(&m_scene);
        setCentralWidget(m_view);
        
        // for(int i = 0; i < 512; i++) {
        //     draw_level(filePath.toLatin1().data(), i);
        //     QImage image(scene.sceneRect().size().toSize(), QImage::Format_ARGB32);
        //     QPainter painter(&image);
        //     painter.setRenderHint(QPainter::SmoothPixmapTransform);
        //     scene.render(&painter, QRectF(0, 0, 256*screens, 432));
        //     image.save("png/" + QString::number(i, 16) + ".png");
        // }

        
        draw_level(m_currentLevel);

        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(8000/60);

    }

    MainWindow::MainWindow() : MainWindow(0x105) { }

    MainWindow::MainWindow(MainWindow& w) :  MainWindow(w.getLevelNumber()) { }

    MainWindow::~MainWindow() {
        // delete m_dockMap8;
        // delete m_dockMap16;
        // delete m_view;
    }

    void MainWindow::update() {

        m_frame += 8;
        for(int i = 0; i < 8; i++) {
            m_level.animate(m_frame + i);
        }
        draw_level(m_currentLevel);
        m_scene.invalidate();
        m_sceneMap8.invalidate();
        m_sceneMap16.invalidate();
    }


    void MainWindow::draw_level(int levelnum) {
        setWindowTitle(QString("Mockup - Level 0x%1").arg(levelnum, 0, 16));
        
        m_scene.clear();
        m_sceneMap8.clear();
        m_sceneMap16.clear();
        m_view->setSceneRect(0, 0, m_level.getWidth() * 16 , m_level.getHeight() * 16);
        m_viewMap8->setSceneRect(0, 0, 128, 256);
        uint32_t palette[256]; 
        memcpy(&palette, m_level.getPalette(), 1024);

        //Background Color
        m_scene.setBackgroundBrush(QBrush(QColor::fromRgba(m_level.getBackgroundColor())));

        //Background
        QImage imgBG(m_level.getWidth()*16, m_level.getHeight()*16, QImage::Format_Indexed8);
        for(int i = 0; i < 256; i++) imgBG.setColor(i, palette[i]);
        imgBG.fill(QColor(0, 0, 0, 0));
           
        for(int i = 0; i < m_level.getHeight()*16; i++) {
            m_level.renderLineBG(imgBG.scanLine(i), i); 
        }
        QGraphicsItem* itemBG = m_scene.addPixmap(QPixmap::fromImage(imgBG));

        //Foreground
        QImage imgFG(m_level.getWidth()*16, m_level.getHeight()*16, QImage::Format_Indexed8);
        for(int i = 0; i < 256; i++) imgFG.setColor(i, palette[i]);
        imgFG.fill(QColor(0, 0, 0, 0));

        for(int i = 0; i < m_level.getHeight()*16; i++) {
            m_level.renderLineFG(imgFG.scanLine(i), i); 
        }
        
        QGraphicsItem* itemFG = m_scene.addPixmap(QPixmap::fromImage(imgFG));

        //Map8
        QImage imgMap8(128, 256, QImage::Format_Indexed8);
        for(int i = 0; i < 256; i++) imgMap8.setColor(i, palette[i]);
        imgBG.fill(QColor(0, 0, 0, 0));
        for(int i = 0; i < 512; i++) {
            Tile8 tile = m_level.getMap8(i);
            for(int j = 0; j < 64; j++) {
                imgMap8.setPixel((i % 16) * 8 + (j % 8), (i / 16) * 8 + (j / 8), tile.pixels[j]);
            }
        }
        QGraphicsItem* itemMap8 = m_sceneMap8.addPixmap(QPixmap::fromImage(imgMap8));

        //Map16
        QImage imgMap16(256, 256, QImage::Format_Indexed8);
        for(int i = 0; i < 256; i++) imgMap16.setColor(i, palette[i]);
        imgBG.fill(QColor(255, 255, 255, 255));
        for(int i = 0; i < 256; i++) {
            Tile16 tile = m_level.getMap16bg(i);
            for(int j = 0; j < 256; j++) {
                imgMap16.setPixel((i % 16) * 16 + (j % 16), (i / 16) * 16 + (j / 16), tile.pixels[j]);
            }
        }
        QGraphicsItem* itemMap16 = m_sceneMap16.addPixmap(QPixmap::fromImage(imgMap16));
    }
    
 
 
    void MainWindow::keyPressEvent(QKeyEvent* event) {
        if(event->key() == Qt::Key_PageUp && m_currentLevel < 511) {
            m_currentLevel++;
        } else if(event->key() == Qt::Key_PageDown && m_currentLevel > 0) {
            m_currentLevel--;
        } else {
            event->ignore();
            return;
        }
        event->accept();
        m_level.load_level(m_currentLevel);
        draw_level(m_currentLevel);
    }


    QString MainWindow::getFilePath() {
        return m_filePath;
    }

    int MainWindow::getLevelNumber() {
        return m_currentLevel;
    }



};
