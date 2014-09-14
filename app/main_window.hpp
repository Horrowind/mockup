#ifndef MOCKUP_MAIN_WINDOW_HPP
#define MOCKUP_MAIN_WINDOW_HPP

#include <vector>

#include <QMainWindow>
#include <QGraphicsView>

#include "testprocessor.hpp"

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
    
    private:

        void draw_level(const char* path, int level);

        void draw_normal_object(const char* path, uint8_t object, uint8_t size, uint8_t pos, uint8_t* subscreen_low_ptr, uint8_t* subscreen_high_ptr);
        void draw_extended_object(const char* path, uint8_t object, uint8_t pos, uint8_t* subscreen_low_ptr, uint8_t* subscreen_high_ptr);
        std::vector<QPixmap> load_map16(const char* path, int level);
        std::vector<QPixmap> load_map8(const char* path, int level, int paletteNumber);
        
        QGraphicsView* view;
        QGraphicsScene scene;
        //TestCPU* cpu;
        
        uint8_t* layer1low = new uint8_t[27 * 16 * 0x14];
        uint8_t* layer1high = new uint8_t[27 * 16 * 0x14];
        int cur_level = 0x105;
        int cur_palette = 0;
        QString filePath;
    };
}
#endif //MOCKUP_MAIN_WINDOW_HPP
