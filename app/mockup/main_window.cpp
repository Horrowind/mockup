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
        
        setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);

        mViewPalette = new QGraphicsView(&mScenePalette);
        mViewPalette->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mViewPalette->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mViewPalette->setMinimumSize(128, 128);
        mViewPalette->setMaximumSize(128, 128);
      
        mDockPalette = new QDockWidget("Palette", this);
        mDockPalette->setWidget(mViewPalette);

        mViewMap8 = new QGraphicsView(&mSceneMap8);
        mViewMap8->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mViewMap8->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mViewMap8->setMinimumSize(128, 256);
        mViewMap8->setMaximumSize(128, 256);
      
        mDockMap8 = new QDockWidget("Map 8", this);
        mDockMap8->setWidget(mViewMap8);
        
        mViewMap16FG = new QGraphicsView(&mSceneMap16FG);
        mViewMap16FG->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mViewMap16FG->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mViewMap16FG->setMinimumSize(256, 512);
        mViewMap16FG->setMaximumSize(256, 512);
      
        mDockMap16FG = new QDockWidget("Map 16 FG", this);
        mDockMap16FG->setWidget(mViewMap16FG);

        mViewMap16BG = new QGraphicsView(&mSceneMap16BG);
        mViewMap16BG->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mViewMap16BG->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mViewMap16BG->setMinimumSize(256, 512);
        mViewMap16BG->setMaximumSize(256, 512);
      
        mDockMap16BG = new QDockWidget("Map 16 BG", this);
        mDockMap16BG->setWidget(mViewMap16BG);
        
        addDockWidget(Qt::RightDockWidgetArea, mDockMap8, Qt::Vertical);
        tabifyDockWidget(mDockMap8, mDockMap16FG);
        tabifyDockWidget(mDockMap8, mDockMap16BG);
        tabifyDockWidget(mDockMap8, mDockPalette);

        
        mView = new QGraphicsView(&mScene);
        mView->setDragMode(QGraphicsView::RubberBandDrag);
        mView->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
        setCentralWidget(mView);

       
        mOpenAct = new QAction(tr("&Open..."), this);
        mOpenAct->setShortcuts(QKeySequence::Open);
        mOpenAct->setStatusTip(tr("Open an existing rom"));
        mQuitAct = new QAction(tr("&Quit"), this);
        mQuitAct->setShortcuts(QKeySequence::Quit);
        mQuitAct->setStatusTip(tr("Quit the application"));
        connect(mOpenAct, SIGNAL(triggered()), this, SLOT(open_rom()));
        connect(mQuitAct, SIGNAL(triggered()), this, SLOT(close()));
        mFileMenu = menuBar()->addMenu(tr("&File"));
        mFileMenu->addAction(mOpenAct);
        mFileMenu->addAction(mQuitAct);

        mIncrementLevelShortcut = new QShortcut(QKeySequence(QKeySequence::MoveToPreviousPage), this);
        mDecrementLevelShortcut = new QShortcut(QKeySequence(QKeySequence::MoveToNextPage), this);
        mZoomInShortcut = new QShortcut(QKeySequence(QKeySequence::ZoomIn), this);
        mZoomOutShortcut = new QShortcut(QKeySequence(QKeySequence::ZoomOut), this);
        mZoomResetShortcut = new QShortcut(QKeySequence(tr("Ctrl+0")), this);
        connect(mIncrementLevelShortcut, SIGNAL(activated()), this, SLOT(increment_level()));
        connect(mDecrementLevelShortcut, SIGNAL(activated()), this, SLOT(decrement_level()));
        connect(mZoomInShortcut, SIGNAL(activated()), this, SLOT(zoom_in()));
        connect(mZoomOutShortcut, SIGNAL(activated()), this, SLOT(zoom_out()));
        connect(mZoomResetShortcut, SIGNAL(activated()), this, SLOT(zoom_reset()));
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
        if(mRomIsLoaded) {
            smw_deinit(&mSmw);
            r65816_rom_free(&mRom);
        }
        delete mDockMap8;
        delete mDockMap16FG;
        delete mDockMap16BG;
        delete mView;
    }

    void MainWindow::update() {
        if(mRomIsLoaded) {
            mFrame += 8;
            for(uint8_t i = 0; i < 8; i++) {
                level_animate(&mSmw.levels[mCurrentLevel], mFrame + i, &mSmw.gfx_pages);
            }
            draw_level();
            mScene.invalidate();
            mSceneMap8.invalidate();
            mSceneMap16FG.invalidate();
            mSceneMap16BG.invalidate();
            mScenePalette.invalidate();
        } else {
            mSceneMap8.clear();
            mSceneMap16FG.clear();
            mSceneMap16BG.clear();
            mScenePalette.clear();
            mScene.clear();

            mScene.invalidate();
            mSceneMap8.invalidate();
            mSceneMap16FG.invalidate();
            mSceneMap16BG.invalidate();
            mScenePalette.invalidate();
        }
    }

    void MainWindow::draw_level() {
        if(mSmw.levels[mCurrentLevel].is_boss_level) {
            setWindowTitle(QString("Mockup - Level %1 - Boss battle").arg(mCurrentLevel, 0, 16));
            return;
        }
        setWindowTitle(QString("Mockup - Level %1").arg(mCurrentLevel, 0, 16));
        mSceneMap8.clear();
        mSceneMap16FG.clear();
        mSceneMap16BG.clear();
        mScenePalette.clear();
        mView->setSceneRect(0, 0, mSmw.levels[mCurrentLevel].width * 16, mSmw.levels[ mCurrentLevel].height * 16);

        // Palette
        palette_to_pc(&mSmw.levels[ mCurrentLevel].palette, &mPalette);
        QImage imgPalette(128, 128, QImage::Format_Indexed8);
        for(int i = 0; i < 256; i++) imgPalette.setColor(i, mPalette.data[i]);
        for(int i = 0; i < 128; i++) {
            for(int j = 0; j < 128; j++) {
                imgPalette.setPixel(j, i, (i/8)*16 + j/8);
            }
        }
        QGraphicsItem* itemPalette = mScenePalette.addPixmap(QPixmap::fromImage(imgPalette));
        
        // Map8
        QImage imgMap8(128, 256, QImage::Format_Indexed8);
        for(int i = 0; i < 256; i++) imgMap8.setColor(i, mPalette.data[i]);
        for(int i = 0; i < 512; i++) {
            for(int j = 0; j < 64; j++) {
                imgMap8.setPixel((i % 16) * 8 + (j % 8), (i / 16) * 8 + (j / 8),
                                 mSmw.levels[ mCurrentLevel].map8.tiles[i].pixels[j]);
            }
        }
        QGraphicsItem* itemMap8 = mSceneMap8.addPixmap(QPixmap::fromImage(imgMap8));
        mViewMap8->setSceneRect(0, 0, 128, 256);

        //Map16 FG
        QImage imgMap16FG(256, 512, QImage::Format_Indexed8);
        for(int i = 0; i < 256; i++) imgMap16FG.setColor(i, mPalette.data[i]);
        map16_pc_init(&mMap16FG, &mSmw.levels[ mCurrentLevel].map16_fg);
        for(int k = 0; k < 512; k++) {
            for(int j = 0; j < 256; j++) {
                imgMap16FG.setPixel((k % 16) * 16 + (j % 16), (k / 16) * 16 + (j / 16), mMap16FG.tiles[k].data[j]);
            }
        }
        
        QGraphicsItem* itemMap16FG = mSceneMap16FG.addPixmap(QPixmap::fromImage(imgMap16FG));

        //Map16 BG
        QImage imgMap16BG(256, 512, QImage::Format_Indexed8);
        for(int i = 0; i < 256; i++) imgMap16BG.setColor(i, mPalette.data[i]);
        map16_pc_init(&mMap16BG, &mSmw.levels[mCurrentLevel].map16_bg);
        for(int k = 0; k < 512; k++) {
            for(int j = 0; j < 256; j++) {
                imgMap16BG.setPixel((k % 16) * 16 + (j % 16), (k / 16) * 16 + (j / 16), mMap16BG.tiles[k].data[j]);
            }
        }
        
        QGraphicsItem* itemMap16BG = mSceneMap16BG.addPixmap(QPixmap::fromImage(imgMap16BG));

        
        // Layer 1 objects
        foreach(QGraphicsPixmapItem* item, mObjectList) {
            object_pc_t* obj = (object_pc_t*) item->data(0).value<object_pc_t*>();
            int obj_width = obj->bb_xmax - obj->bb_xmin + 1;
            int obj_height = obj->bb_ymax - obj->bb_ymin + 1;
            
            QImage img(obj_width * 16, obj_height * 16, QImage::Format_Indexed8);
            for(int j = 0; j < 256; j++) img.setColor(j, mPalette.data[j]);
            for(int y = 0; y < obj_height; y++) {
                for(int x = 0; x < obj_width; x++) {
                    for(int j = 0; j < 256; j++) {
                        img.setPixel(x * 16 + (j % 16), y * 16 + (j / 16), mMap16FG.tiles[obj->tiles[y * obj_width + x]].data[j]);
                    }
                }
            }
            item->setPixmap(QPixmap::fromImage(img));
        }

        map16_pc_deinit(&mMap16FG);
        map16_pc_deinit(&mMap16BG);

    }
    
    void MainWindow::increment_level() {
        if(mRomIsLoaded && mCurrentLevel < 511) {
            mCurrentLevel++;
            load_level();
        }
    }
    
 
    void MainWindow::decrement_level() {
        if(mRomIsLoaded && mCurrentLevel > 0) {
            mCurrentLevel--;
            load_level();
        }
    }

    void MainWindow::open_rom() {
        if(mRomIsLoaded) {
            mRomIsLoaded = false;
            smw_deinit(&mSmw);
            r65816_rom_free(&mRom);
        }
        mFilePath = QFileDialog::getOpenFileName(this, tr("Open Rom"), 0, tr("Rom Files (*.smc *.sfc)"));
        r65816_rom_load(&mRom, mFilePath.toLatin1().data());
        smw_init(&mSmw, &mRom);
        mRomIsLoaded = true;
        load_level();
    }
    
    void MainWindow::load_level() {
        mScene.clear();
        mScene.setSceneRect(0, 0, mSmw.levels[mCurrentLevel].width * 16 , mSmw.levels[mCurrentLevel].height * 16);
        
        smw_level_load(&mSmw, mCurrentLevel);
        mScene.setBackgroundBrush(QBrush(QColor::fromRgba(mSmw.levels[mCurrentLevel].background_color)));
        mObjectList.clear();
        for(int i = 0; i < mSmw.levels[mCurrentLevel].layer1_objects.length; i++) {
            object_pc_t* obj = &mSmw.levels[mCurrentLevel].layer1_objects.objects[i];
            if(!obj->tiles) continue;
            QGraphicsPixmapItem* item = mScene.addPixmap(QPixmap());
            item->setData(0, QVariant::fromValue(obj));
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            item->setFlag(QGraphicsItem::ItemIsMovable, true);
            item->setPos(obj->bb_xmin * 16, obj->bb_ymin * 16);
            //item->setZValue(obj->zindex);
            mObjectList.append(item);
        }

        // Layer 2 background
        
        if(mSmw.levels[mCurrentLevel].has_layer2_bg) {
            map16_pc_t map16BG;
            palette_pc_t palette;
            palette_to_pc(&mSmw.levels[mCurrentLevel].palette, &palette);
            map16_pc_init(&map16BG, &mSmw.levels[mCurrentLevel].map16_bg);
            QImage imgLayer2BG(mSmw.levels[mCurrentLevel].width * 16, mSmw.levels[mCurrentLevel].height * 16, QImage::Format_Indexed8);
            for(int i = 0; i < 256; i++) imgLayer2BG.setColor(i, palette.data[i]);
            for(int k = 0; k < mSmw.levels[mCurrentLevel].height * 16; k++) {
                int gy = (k >> 4) % 27;
                int by = k % 16;
                for(int i = 0; i < mSmw.levels[mCurrentLevel].width * 16; i++) {
                    int pos = by * 16 + i % 16;
                    imgLayer2BG.setPixel(i, k, map16BG.tiles[
                                             mSmw.levels[mCurrentLevel]
                                             .layer2_background
                                             .data[((i & 0x100) >> 4) * 27 + gy * 16 + ((i & 0x0FF) >> 4)]
                                             ].data[pos]);
                }
            }
            mBackgroundItem = mScene.addPixmap(QPixmap::fromImage(imgLayer2BG));
            mBackgroundItem->setZValue(-255);
            map16_pc_deinit(&map16BG);
        } else if(mSmw.levels[mCurrentLevel].has_layer2_objects) {

            // TODO: Implement Layer 2 Objects.
        }
        update();
        mScene.update();
    }


    const float zoom_in_value = pow(2.0, 1.0 / 8);
    const float zoom_out_value = pow(2.0, -1.0 / 8);
    
    void MainWindow::zoom_in() {
        mView->scale(zoom_in_value, zoom_in_value);
    }
    void MainWindow::zoom_out() {
        mView->scale(zoom_out_value, zoom_out_value);
    }

    void MainWindow::zoom_reset() {
        mView->setTransform(QTransform());
    }

    void MainWindow::objects_changed() {
        int z_index = 0;
        foreach(QGraphicsItem* item, mScene.items(Qt::AscendingOrder)) {
            if(item->zValue() == 0) {
                object_pc_t* obj = item->data(0).value<object_pc_t*>();
                //obj->
                z_index++;
            }
        }
    }
    
};


