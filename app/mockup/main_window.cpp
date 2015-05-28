#include <cstdint>
#include <cstdio>
#include <cstring>

#include <QDockWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QTimer>

#include "main_window.hpp"



namespace Mockup {
    

    MainWindow::MainWindow() {

        setDocumentMode(true);
        grabKeyboard();
        
        setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);

        m_dockDebug = new QDockWidget("Debug", this);
        m_textDebug = new QTextEdit(m_dockDebug);
        m_textDebug->setReadOnly(true);
        m_dockDebug->setWidget(m_textDebug);

        m_viewMap8 = new QGraphicsView(&m_sceneMap8);
        m_viewMap8->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_viewMap8->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_viewMap8->setMinimumSize(128, 256);
        m_viewMap8->setMaximumSize(128, 256);
      
        m_dockMap8 = new QDockWidget("Map 8", this);
        m_dockMap8->setWidget(m_viewMap8);
        
        m_viewMap16FG = new QGraphicsView(&m_sceneMap16FG);
        m_viewMap16FG->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_viewMap16FG->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_viewMap16FG->setMinimumSize(256, 256);
        m_viewMap16FG->setMaximumSize(256, 256);
      
        m_dockMap16FG = new QDockWidget("Map 16 FG", this);
        m_dockMap16FG->setWidget(m_viewMap16FG);

        m_viewMap16BG = new QGraphicsView(&m_sceneMap16BG);
        m_viewMap16BG->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_viewMap16BG->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_viewMap16BG->setMinimumSize(256, 256);
        m_viewMap16BG->setMaximumSize(256, 256);
      
        m_dockMap16BG = new QDockWidget("Map 16 BG", this);
        m_dockMap16BG->setWidget(m_viewMap16BG);
        
        addDockWidget(Qt::BottomDockWidgetArea, m_dockMap8);
        addDockWidget(Qt::BottomDockWidgetArea, m_dockMap16FG);
        addDockWidget(Qt::BottomDockWidgetArea, m_dockMap16BG);
        addDockWidget(Qt::BottomDockWidgetArea, m_dockDebug);
        m_view = new QGraphicsView(&m_scene);
        setCentralWidget(m_view);
        m_filePath = "smw.sfc";
        r65816_rom_load(&m_rom, m_filePath.toLatin1().data());
        smw_init(&m_smw, &m_rom);
        
        
        // for(int i = 0; i < 512; i++) {
        //     draw_level(filePath.toLatin1().data(), i);
        //     QImage image(scene.sceneRect().size().toSize(), QImage::Format_ARGB32);
        //     QPainter painter(&image);
        //     painter.setRenderHint(QPainter::SmoothPixmapTransform);
        //     scene.render(&painter, QRectF(0, 0, 256*screens, 432));
        //     image.save("png/" + QString::number(i, 16) + ".png");
        // }


        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(8000/60);
        
    }
    MainWindow::~MainWindow() {
        smw_deinit(&m_smw);
        r65816_rom_free(&m_rom);
        delete m_dockMap8;
        delete m_dockMap16FG;
        delete m_dockMap16BG;
        delete m_view;
    }

    void MainWindow::update() {
        m_frame += 8;
        draw_level(m_currentLevel);
        m_scene.invalidate();
        m_sceneMap8.invalidate();
        m_sceneMap16FG.invalidate();
        m_sceneMap16BG.invalidate();
    }


    void MainWindow::draw_level(int levelnum) {
        smw_level_load(&m_smw, levelnum);
        for(uint8_t i = 0; i < 8; i++) {
            level_animate(&m_smw.levels[m_currentLevel], m_frame + i, &m_smw.gfx_pages);
        }
        setWindowTitle(QString("Mockup - Level %1").arg(levelnum, 0, 16));
        m_scene.clear();
        m_sceneMap8.clear();
        m_sceneMap16FG.clear();
        m_sceneMap16BG.clear();

        m_view->setSceneRect(0, 0, m_smw.levels[levelnum].width * 16, m_smw.levels[levelnum].height * 16);

        // Palette
        palette_pc_t palette_pc;
        palette_to_pc(&m_smw.levels[levelnum].palette, &palette_pc);
        
        // Map8
        QImage imgMap8(128, 256, QImage::Format_Indexed8);
        for(int i = 0; i < 256; i++) imgMap8.setColor(i, palette_pc.data[i]);
        for(int i = 0; i < 512; i++) {
            for(int j = 0; j < 64; j++) {
                imgMap8.setPixel((i % 16) * 8 + (j % 8), (i / 16) * 8 + (j / 8),
                                 m_smw.levels[levelnum].map8.tiles[i].pixels[j]);
            }
        }
        QGraphicsItem* itemMap8 = m_sceneMap8.addPixmap(QPixmap::fromImage(imgMap8));
        m_viewMap8->setSceneRect(0, 0, 128, 256);

        //Map16 FG
        QImage imgMap16FG(256, 256, QImage::Format_Indexed8);
        for(int i = 0; i < 256; i++) imgMap16FG.setColor(i, palette_pc.data[i]);
        for(int k = 0; k < 256; k++) {
            tile16_t tile = m_smw.levels[levelnum].map16_fg.tiles[k];
            for(int i = 0; i < 8; i++) {
                for(int j = 0; j < 8; j++) {
                    imgMap16FG.setPixel((k % 16) * 16 + j, (k / 16) * 16 + i,
                                      16 * tile.properties[0].CCC + tile.tile8s[0]->pixels[
                                          (tile.properties[0].Y ? (7 - i) * 8 : i * 8)
                                          + (tile.properties[0].X ? 7 - j : j)]);
                    imgMap16FG.setPixel((k % 16) * 16 + j + 8, (k / 16) * 16 + i,
                                      16 * tile.properties[2].CCC + tile.tile8s[2]->pixels[
                                          (tile.properties[2].Y ? (7 - i) * 8 : i * 8)
                                          + (tile.properties[2].X ? 7 - j : j)]);
                    imgMap16FG.setPixel((k % 16) * 16 + j, (k / 16) * 16 + i + 8,
                                      16 * tile.properties[1].CCC + tile.tile8s[1]->pixels[
                                          (tile.properties[1].Y ? (7 - i) * 8 : i * 8)
                                          + (tile.properties[1].X ? 7 - j : j)]);
                    imgMap16FG.setPixel((k % 16) * 16 + j + 8, (k / 16) * 16 + i + 8,
                                      16 * tile.properties[3].CCC + tile.tile8s[3]->pixels[
                                          (tile.properties[3].Y ? (7 - i) * 8 : i * 8)
                                          + (tile.properties[3].X ? 7 - j : j)]);
                }
            }
        }
        QGraphicsItem* itemMap16FG = m_sceneMap16FG.addPixmap(QPixmap::fromImage(imgMap16FG));

        //Map16 BG
        QImage imgMap16BG(256, 256, QImage::Format_Indexed8);
        for(int i = 0; i < 256; i++) imgMap16BG.setColor(i, palette_pc.data[i]);
        for(int k = 0; k < 256; k++) {
            tile16_t tile = m_smw.levels[levelnum].map16_bg.tiles[k];
            for(int i = 0; i < 8; i++) {
                for(int j = 0; j < 8; j++) {
                    imgMap16BG.setPixel((k % 16) * 16 + j, (k / 16) * 16 + i,
                                      16 * tile.properties[0].CCC + tile.tile8s[0]->pixels[
                                          (tile.properties[0].Y ? (7 - i) * 8 : i * 8)
                                          + (tile.properties[0].X ? 7 - j : j)]);
                    imgMap16BG.setPixel((k % 16) * 16 + j + 8, (k / 16) * 16 + i,
                                      16 * tile.properties[2].CCC + tile.tile8s[2]->pixels[
                                          (tile.properties[2].Y ? (7 - i) * 8 : i * 8)
                                          + (tile.properties[2].X ? 7 - j : j)]);
                    imgMap16BG.setPixel((k % 16) * 16 + j, (k / 16) * 16 + i + 8,
                                      16 * tile.properties[1].CCC + tile.tile8s[1]->pixels[
                                          (tile.properties[1].Y ? (7 - i) * 8 : i * 8)
                                          + (tile.properties[1].X ? 7 - j : j)]);
                    imgMap16BG.setPixel((k % 16) * 16 + j + 8, (k / 16) * 16 + i + 8,
                                      16 * tile.properties[3].CCC + tile.tile8s[3]->pixels[
                                          (tile.properties[3].Y ? (7 - i) * 8 : i * 8)
                                          + (tile.properties[3].X ? 7 - j : j)]);
                }
            }
        }
        QGraphicsItem* itemMap16BG = m_sceneMap16BG.addPixmap(QPixmap::fromImage(imgMap16BG));

        // Layer 1 objects
        
        for(int i = 0; i < m_smw.levels[levelnum].layer1_objects.length; i++) {


        }


    }


        
    //     //Background Color
    //     m_scene.setBackgroundBrush(QBrush(QColor::fromRgba(m_level.getBackgroundColor())));

    //     //Background
    //     QImage imgBG(m_level.getWidth()*16, m_level.getHeight()*16, QImage::Format_Indexed8);
    //     for(int i = 0; i < 256; i++) imgBG.setColor(i, palette[i]);
    //     imgBG.fill(QColor(0, 0, 0, 0));
           
    //     for(int i = 0; i < m_level.getHeight()*16; i++) {
    //         m_level.renderLineBG(imgBG.scanLine(i), i); 
    //     }
    //     QGraphicsItem* itemBG = m_scene.addPixmap(QPixmap::fromImage(imgBG));

    //     //Foreground
    //     QImage imgFG(m_level.getWidth()*16, m_level.getHeight()*16, QImage::Format_Indexed8);
    //     for(int i = 0; i < 256; i++) imgFG.setColor(i, palette[i]);
    //     imgFG.fill(QColor(0, 0, 0, 0));

    //     for(int i = 0; i < m_level.getHeight()*16; i++) {
    //         m_level.renderLineFG(imgFG.scanLine(i), i); 
    //     }
        
    //     QGraphicsItem* itemFG = m_scene.addPixmap(QPixmap::fromImage(imgFG));


    
 
 
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
    }

};
