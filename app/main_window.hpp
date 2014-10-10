#ifndef MOCKUP_MAIN_WINDOW_HPP
#define MOCKUP_MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QGraphicsView>

#include "cpu.hpp"
#include "level.hpp"

class TestCPU;
class QKeyEvent;

namespace Mockup {
    
    class MainWindow : public QMainWindow {
        Q_OBJECT
    public:
        MainWindow();
        ~MainWindow();

    protected:
        virtual void keyPressEvent(QKeyEvent* event);
    
    public slots:
        void update();
        
        
    private:
        
        void draw_level(int level);

        QGraphicsView* view;
        QGraphicsScene scene;
        
        int cur_level = 0x105;
        uint8_t m_frame = 0;
        Level m_level;
        QString filePath;
    };
}
#endif //MOCKUP_MAIN_WINDOW_HPP
