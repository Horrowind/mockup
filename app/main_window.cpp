#include <cstdint>
#include <cstdio>
#include <cstring>

#include <iostream>
#include <fstream>
#include <QFileDialog>
#include <QInputDialog>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QTimer>



#include "cpu.hpp"
#include "level.hpp"
#include "main_window.hpp"


#define WORLDLIB_IGNORE_DLL_FUNCTIONS
#include "WorldLib.hpp"


namespace Mockup {
    

    MainWindow::MainWindow() : m_level("smw.smc", 0x105) {
        // filePath = QFileDialog::getOpenFileName(nullptr, "Open ROM", QString(), "*.smc");
        // if(filePath == "") exit(0);
        
        setDocumentMode(true);
        setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);
        view = new QGraphicsView(&scene);
        grabKeyboard();
        MainWindow::setCentralWidget(view);
        // for(int i = 0; i < 512; i++) {
        //     draw_level(filePath.toLatin1().data(), i);
        //     QImage image(scene.sceneRect().size().toSize(), QImage::Format_ARGB32);
        //     QPainter painter(&image);
        //     painter.setRenderHint(QPainter::SmoothPixmapTransform);
        //     scene.render(&painter, QRectF(0, 0, 256*screens, 432));
        //     image.save("png/" + QString::number(i, 16) + ".png");
        // }

        
        draw_level(cur_level);

        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	// timer->start(50);

    }
    
    MainWindow::~MainWindow() {
        
    }

    void MainWindow::update() {
        m_frame++;
        m_level.animate(m_frame);
        draw_level(cur_level);
    }


    void MainWindow::draw_level(int levelnum) {
        scene.clear();
        QImage img(m_level.getWidth()*16, m_level.getHeight()*16, QImage::Format_Indexed8);
        img.fill(QColor(0, 0, 0, 0));

        uint32_t palette[256];
        memcpy(&palette, m_level.getPalette(), 1024);
        for(int i = 0; i < 256; i++) img.setColor(i, palette[i]);

        for(int i = 0; i < m_level.getHeight()*16; i++) {
            m_level.renderLine(img.scanLine(i), i); 
        }
        
        QGraphicsItem* item = scene.addPixmap(QPixmap::fromImage(img));
    }
    
 
 
    void MainWindow::keyPressEvent(QKeyEvent* event) {
        if(event->key() == Qt::Key_PageUp && cur_level < 511) {
            cur_level++;
        } else if(event->key() == Qt::Key_PageDown && cur_level > 0) {
            cur_level--;
        } else {
            event->ignore();
            return;
        }
        event->accept();
        m_level.load_level(cur_level);
        draw_level(cur_level);
    }
 


};
