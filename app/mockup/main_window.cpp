#include <cstdint>
#include <cstdio>
#include <cstring>

#include <QDockWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QTimer>
#include <QThread>

//#include "graphics_object.hpp"
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

        m_viewPalette = new QGraphicsView(&m_scenePalette);
        m_viewPalette->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_viewPalette->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_viewPalette->setMinimumSize(128, 128);
        m_viewPalette->setMaximumSize(128, 128);
      
        m_dockPalette = new QDockWidget("Palette", this);
        m_dockPalette->setWidget(m_viewPalette);

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
        m_viewMap16FG->setMinimumSize(256, 512);
        m_viewMap16FG->setMaximumSize(256, 512);
      
        m_dockMap16FG = new QDockWidget("Map 16 FG", this);
        m_dockMap16FG->setWidget(m_viewMap16FG);

        m_viewMap16BG = new QGraphicsView(&m_sceneMap16BG);
        m_viewMap16BG->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_viewMap16BG->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_viewMap16BG->setMinimumSize(256, 512);
        m_viewMap16BG->setMaximumSize(256, 512);
      
        m_dockMap16BG = new QDockWidget("Map 16 BG", this);
        m_dockMap16BG->setWidget(m_viewMap16BG);
        
        addDockWidget(Qt::BottomDockWidgetArea, m_dockMap8);
        addDockWidget(Qt::BottomDockWidgetArea, m_dockMap16FG);
        addDockWidget(Qt::BottomDockWidgetArea, m_dockMap16BG);
        addDockWidget(Qt::BottomDockWidgetArea, m_dockPalette);
        addDockWidget(Qt::BottomDockWidgetArea, m_dockDebug);
        m_view = new QGraphicsView(&m_scene);
        m_view->setDragMode(QGraphicsView::RubberBandDrag);
        m_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
        setCentralWidget(m_view);
        m_filePath = "smw.sfc";
        r65816_rom_load(&m_rom, m_filePath.toLatin1().data());
        smw_init(&m_smw, &m_rom);

        load_level();
        
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
        for(uint8_t i = 0; i < 8; i++) {
            level_animate(&m_smw.levels[m_currentLevel], m_frame + i, &m_smw.gfx_pages);
        }

        draw_level();
        m_scene.invalidate();
        m_sceneMap8.invalidate();
        m_sceneMap16FG.invalidate();
        m_sceneMap16BG.invalidate();
        m_scenePalette.invalidate();
    }


    void MainWindow::draw_level() {
        
        setWindowTitle(QString("Mockup - Level %1").arg( m_currentLevel, 0, 16));
        m_sceneMap8.clear();
        m_sceneMap16FG.clear();
        m_sceneMap16BG.clear();
        m_scenePalette.clear();
        m_view->setSceneRect(0, 0, m_smw.levels[m_currentLevel].width * 16, m_smw.levels[ m_currentLevel].height * 16);

        // Palette
        palette_to_pc(&m_smw.levels[ m_currentLevel].palette, &m_palette);
        QImage imgPalette(128, 128, QImage::Format_Indexed8);
        for(int i = 0; i < 256; i++) imgPalette.setColor(i, m_palette.data[i]);
        for(int i = 0; i < 128; i++) {
            for(int j = 0; j < 128; j++) {
                imgPalette.setPixel(j, i, (i/8)*16 + j/8);
            }
        }
        QGraphicsItem* itemPalette = m_scenePalette.addPixmap(QPixmap::fromImage(imgPalette));
        
        // Map8
        QImage imgMap8(128, 256, QImage::Format_Indexed8);
        for(int i = 0; i < 256; i++) imgMap8.setColor(i, m_palette.data[i]);
        for(int i = 0; i < 512; i++) {
            for(int j = 0; j < 64; j++) {
                imgMap8.setPixel((i % 16) * 8 + (j % 8), (i / 16) * 8 + (j / 8),
                                 m_smw.levels[ m_currentLevel].map8.tiles[i].pixels[j]);
            }
        }
        QGraphicsItem* itemMap8 = m_sceneMap8.addPixmap(QPixmap::fromImage(imgMap8));
        m_viewMap8->setSceneRect(0, 0, 128, 256);

        //Map16 FG
        QImage imgMap16FG(256, 512, QImage::Format_Indexed8);
        for(int i = 0; i < 256; i++) imgMap16FG.setColor(i, m_palette.data[i]);
        map16_pc_init(&m_map16FG, &m_smw.levels[ m_currentLevel].map16_fg);
        for(int k = 0; k < 512; k++) {
            for(int j = 0; j < 256; j++) {
                imgMap16FG.setPixel((k % 16) * 16 + (j % 16), (k / 16) * 16 + (j / 16), m_map16FG.tiles[k].data[j]);
            }
        }
        
        QGraphicsItem* itemMap16FG = m_sceneMap16FG.addPixmap(QPixmap::fromImage(imgMap16FG));

        //Map16 BG
        QImage imgMap16BG(256, 512, QImage::Format_Indexed8);
        for(int i = 0; i < 256; i++) imgMap16BG.setColor(i, m_palette.data[i]);
        map16_pc_init(&m_map16BG, &m_smw.levels[ m_currentLevel].map16_bg);
        for(int k = 0; k < 512; k++) {
            for(int j = 0; j < 256; j++) {
                imgMap16BG.setPixel((k % 16) * 16 + (j % 16), (k / 16) * 16 + (j / 16), m_map16BG.tiles[k].data[j]);
            }
        }
        
        QGraphicsItem* itemMap16BG = m_sceneMap16BG.addPixmap(QPixmap::fromImage(imgMap16BG));

        // Layer 1 objects

        //m_scene.setBackgroundBrush(QBrush(QColor::fromRgba(m_level.getBackgroundColor())));
        foreach(QGraphicsItem* item, m_scene.items()) {
            object_t* obj = (object_t*) item->data(0).value<object_t*>();
            int obj_width = obj->bb_xmax - obj->bb_xmin + 1;
            int obj_height = obj->bb_ymax - obj->bb_ymin + 1;
            
            QImage img(obj_width * 16, obj_height * 16, QImage::Format_Indexed8);
            for(int j = 0; j < 256; j++) img.setColor(j, m_palette.data[j]);
            for(int y = 0; y < obj_height; y++) {
                for(int x = 0; x < obj_width; x++) {
                    for(int j = 0; j < 256; j++) {
                        img.setPixel(x * 16 + (j % 16), y * 16 + (j / 16), m_map16FG.tiles[obj->tiles[y * obj_width + x]].data[j]);
                    }
                }
            }
            QGraphicsPixmapItem* pixitem = qgraphicsitem_cast<QGraphicsPixmapItem*>(item);
            pixitem->setPixmap(QPixmap::fromImage(img));
        }
        
        map16_pc_deinit(&m_map16FG);
        map16_pc_deinit(&m_map16BG);
    }


        
    //     //Background Color
    //     

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
            load_level();
        } else if(event->key() == Qt::Key_PageDown && m_currentLevel > 0) {
            m_currentLevel--;
            load_level();
        } else {
            event->ignore();
            return;
        }
        event->accept();
    }

    void MainWindow::load_level() {
        m_scene.clear();
        m_scene.setSceneRect(0, 0, m_smw.levels[m_currentLevel].width * 16 , m_smw.levels[m_currentLevel].height * 16);
        smw_level_load(&m_smw, m_currentLevel);
        for(int i = 0; i < m_smw.levels[m_currentLevel].layer1_objects.length; i++) {
            object_t* obj = &m_smw.levels[m_currentLevel].layer1_objects.objects[i];
            if(!obj->tiles) continue;
            QGraphicsItem* item = m_scene.addPixmap(QPixmap());
            item->setData(0, QVariant::fromValue(obj));
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            item->setFlag(QGraphicsItem::ItemIsMovable, true);
            item->setPos(obj->bb_xmin * 16, obj->bb_ymin * 16);
        }
        m_scene.update();
    }


};


