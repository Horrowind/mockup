#ifndef MOCKUP_MAIN_WINDOW_HPP
#define MOCKUP_MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QGraphicsView>
#include <QTextEdit>

extern "C" {
#include "smw.h"
}

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
        
        //void draw_level(int level);

        QGraphicsView* m_view;
        QGraphicsScene m_scene;

        QDockWidget*   m_dockDebug;
        QTextEdit*     m_textDebug;

        QDockWidget*   m_dockMap8;
        QGraphicsView* m_viewMap8;
        QGraphicsScene m_sceneMap8;

        QDockWidget*   m_dockMap16;
        QGraphicsView* m_viewMap16;
        QGraphicsScene m_sceneMap16;

        QString        m_filePath;

        smw_t smw;
    };
}
#endif //MOCKUP_MAIN_WINDOW_HPP
