#ifndef QLIGHTBOXWIDGET_H
#define QLIGHTBOXWIDGET_H

#include <QWidget>

class QMessageOverlay : public QWidget
{
	Q_OBJECT

public:
    explicit QMessageOverlay(QWidget* _parent, bool _folowToHeadWidget = false);

protected:
	bool eventFilter(QObject* _object, QEvent* _event);
	void paintEvent(QPaintEvent* _event);
	void showEvent(QShowEvent* _event);

private:
	void updateSelf();
	bool m_isInUpdateSelf;
	QPixmap grabParentWidgetPixmap() const;
	QPixmap m_parentWidgetPixmap;
};

#endif // QLIGHTBOXWIDGET_H
