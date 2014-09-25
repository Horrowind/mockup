#ifndef MOCKUP_MAIN_WINDOW_HPP
#define MOCKUP_MAIN_WINDOW_HPP

#include <vector>

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

        // std::vector<QPixmap> load_map16(const char* path, int level, std::vector<QImage> map8  ,std::vector<uint32_t> palette );
        // std::vector<QImage> load_map8(const char* path, int level);
        // std::vector<uint32_t> load_palette(const char* path, int level);
        
        QGraphicsView* view;
        QGraphicsScene scene;
        
        int cur_level = 0x105;
        uint8_t m_frame = 0;
        Level m_level;
        QString filePath;
    };
}
#endif //MOCKUP_MAIN_WINDOW_HPP
