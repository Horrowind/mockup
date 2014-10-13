
#ifndef MOCKUP_DEBUG_HPP
#define MOCKUP_DEBUG_HPP
#pragma "once"

#include "QtDebug"
#include "QTextEdit"

namespace Mockup {
    namespace Internal {
        struct helper {
        public:
            static QTextEdit* edit;
        };
        QTextEdit* helper::edit;
    }

    void initializeDebug(QTextEdit* edit) {
        Internal::helper::edit = edit;
        qInstallMessageHandler([](QtMsgType type, const QMessageLogContext &context, const QString &msg) {
                QByteArray localMsg = msg.toLocal8Bit();
                switch (type) {
                case QtDebugMsg:
                    Internal::helper::edit->append(QString("Debug: %1 (%2:%3, %4)\n").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
                    break;
                case QtWarningMsg:
                    Internal::helper::edit->append(QString("Warning: %1 (%2:%3, %4)\n").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
                    break;
                case QtCriticalMsg:
                    Internal::helper::edit->append(QString("Critical: %1 (%2:%3, %4)\n").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
                    break;
                case QtFatalMsg:
                    Internal::helper::edit->append(QString("Fatal: %s (%s:%u, %s)\n").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
                    exit(-1);
                }
        });
    }
    

}


#endif //MOCKUP_DEBUG_HPP
