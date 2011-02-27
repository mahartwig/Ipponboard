#ifndef SCALED_TEXT_HEADER_INCLUDED
#define SCALED_TEXT_HEADER_INCLUDED

#include <QFont>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
QT_END_NAMESPACE

class ScaledText : public QWidget
{
	Q_OBJECT

public:
	ScaledText( QWidget* pParent = 0 );
//	explicit ScaledText( const QFont& font, QWidget* pParent = 0 );

	enum ETextSize
	{
		eSize_normal = 0,
		eSize_uppercase,
		eSize_full,
		eSize_MAX
	};

	void SetText( const QString& text, ETextSize size = eSize_normal, bool rotate = false ); // has to be called after setting the font
	void SetFont( const QFont& font );  // will call ReDraw() - default setFont() does not!
	void SetFontAndColor( const QFont& font, const QColor& textColor,
						  const QColor& bgColor = Qt::transparent );
	void SetColor( const QColor& textColor );
	void SetColor( const QColor& textColor, const QColor& bgColor );

	const QColor& GetColor() const;
	const QColor& GetBgColor() const;

	ETextSize GetSize() const;

	void SetBlinking( bool blink, int delay = 750 );
	bool IsBlinking() const;

	void Redraw();

	void setAlignment( Qt::AlignmentFlag flag )
	{
		m_Alignment = flag;
	}

protected:
	void paintEvent( QPaintEvent* event );
	void timerEvent( QTimerEvent* event );

private:
	void set_size( ETextSize size );

	QString m_Text;
	QColor m_TextColor;
	QColor m_BGColor;
	Qt::AlignmentFlag m_Alignment;
	QTextLayout* m_pLayout;
	int m_timerId;
	int m_timerDelay;
	ETextSize m_textSize;
	bool m_isVisible;
	bool m_rotate;
};


#endif // SCALED_TEXT_HEADER_INCLUDED
