#ifndef MOCKUP_MAIN_WINDOW_HPP
#define MOCKUP_MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QMenuBar>
#include <QGraphicsView>
#include <QShortcut>
#include <QTextEdit>

//#include "graphics_object.hpp"

extern "C" {
#include "smw.h"
}
Q_DECLARE_METATYPE(object_pc_t*)
class QKeyEvent;

namespace Mockup {
    
    class MainWindow : public QMainWindow {
        Q_OBJECT
    public:
        MainWindow();
        // MainWindow(int level);
        // MainWindow(MainWindow& w);

        ~MainWindow();

    public slots:
        void update();
        void open_rom();
        void draw_level();
        void load_level();
        void increment_level();
        void decrement_level();
        
        
    private:

        
        
        QMenu*         mFileMenu;
        QAction*       mOpenAct;
        QAction*       mQuitAct;

        QShortcut*     mIncrementLevelShortcut;
        QShortcut*     mDecrementLevelShortcut;
        
        QGraphicsView* mView;
        QGraphicsScene mScene;
        
        QDockWidget*   mDockPalette;
        QGraphicsView* mViewPalette;
        QGraphicsScene mScenePalette;
        
        QDockWidget*   mDockMap8;
        QGraphicsView* mViewMap8;
        QGraphicsScene mSceneMap8;

        QDockWidget*   mDockMap16FG;
        QGraphicsView* mViewMap16FG;
        QGraphicsScene mSceneMap16FG;

        QDockWidget*   mDockMap16BG;
        QGraphicsView* mViewMap16BG;
        QGraphicsScene mSceneMap16BG;

        QList<QGraphicsPixmapItem*> mObjectList;
        QGraphicsPixmapItem* mBackgroundItem;
        
        QString        mFilePath;

        int            mCurrentLevel = 0xE5;
        int            mFrame;

        smw_t          mSmw;
        r65816_rom_t   mRom;
        bool           mRomIsLoaded = false;
        palette_pc_t   mPalette;
        map16_pc_t     mMap16FG;
        map16_pc_t     mMap16BG;

    };
}
#endif //MOCKUP_MAIN_WINDOW_HPP
