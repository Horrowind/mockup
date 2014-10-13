#ifndef MOCKUP_MAIN_WINDOW_HPP
#define MOCKUP_MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QGraphicsView>
#include <QTextEdit>


#include "cpu.hpp"
#include "level.hpp"

class TestCPU;
class QKeyEvent;

namespace Mockup {
    
    class MainWindow : public QMainWindow {
        Q_OBJECT
    public:
        MainWindow();
        MainWindow(int level);
        MainWindow(MainWindow& w);

        ~MainWindow();
        QTextEdit*     m_textDebug;


        int getLevelNumber();
        QString getFilePath();

    protected:
        virtual void keyPressEvent(QKeyEvent* event);
    
    public slots:
        void update();
        
        
    private:
        
        void draw_level(int level);

        QGraphicsView* m_view;
        QGraphicsScene m_scene;

        QDockWidget*   m_dockDebug;

        QDockWidget*   m_dockMap8;
        QGraphicsView* m_viewMap8;
        QGraphicsScene m_sceneMap8;

        QDockWidget*   m_dockMap16;
        QGraphicsView* m_viewMap16;
        QGraphicsScene m_sceneMap16;


        int            m_currentLevel = 0x105;
        uint8_t        m_frame = 0;
        Level          m_level;
        QString        m_filePath;
    };
}
#endif //MOCKUP_MAIN_WINDOW_HPP
