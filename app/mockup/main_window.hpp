#ifndef MOCKUP_MAIN_WINDOW_HPP
#define MOCKUP_MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QGraphicsView>
#include <QTextEdit>

//#include "graphics_object.hpp"

extern "C" {
#include "smw.h"
}
Q_DECLARE_METATYPE(object_t*)
class QKeyEvent;

namespace Mockup {
    
    class MainWindow : public QMainWindow {
        Q_OBJECT
    public:
        MainWindow();
        MainWindow(int level);
        MainWindow(MainWindow& w);

        ~MainWindow();

        int getLevelNumber();
        QString getFilePath();

    protected:
        virtual void keyPressEvent(QKeyEvent* event);
    
    public slots:
        void update();
        
        
    private:
        
        void draw_level();
        void load_level();

        QGraphicsView* m_view;
        QGraphicsScene m_scene;

        QDockWidget*   m_dockDebug;
        QTextEdit*     m_textDebug;

        QDockWidget*   m_dockPalette;
        QGraphicsView* m_viewPalette;
        QGraphicsScene m_scenePalette;
        
        QDockWidget*   m_dockMap8;
        QGraphicsView* m_viewMap8;
        QGraphicsScene m_sceneMap8;

        QDockWidget*   m_dockMap16FG;
        QGraphicsView* m_viewMap16FG;
        QGraphicsScene m_sceneMap16FG;

        QDockWidget*   m_dockMap16BG;
        QGraphicsView* m_viewMap16BG;
        QGraphicsScene m_sceneMap16BG;

        
        QString        m_filePath;

        int            m_currentLevel = 0xC2;
        int            m_frame;

        smw_t          m_smw;
        r65816_rom_t   m_rom;
        palette_pc_t   m_palette;
        map16_pc_t     m_map16FG;
        map16_pc_t     m_map16BG;

    };
}
#endif //MOCKUP_MAIN_WINDOW_HPP
