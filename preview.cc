#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <wx/wx.h>

#define Method2

class MyFrame : public wxFrame
{
public:
	MyFrame(wxFrame *parent, int n, wxString title, wxPoint pos, wxSize size)
		: wxFrame(parent, n, title, pos, size)
#ifdef Method2
		, timer(this, 1)
#endif
{
	FILE *fp = fopen("rom.dat", "rb");
	fread(odata, sizeof(odata), 1, fp);
	fclose(fp);
	timer.Start(200);
}

#ifdef Method2
    wxTimer timer;
	void OnTimer(wxTimerEvent& event);
#endif

	int currentFrame;
	int xOffset, yOffset;
	unsigned char odata[256 * 32];

	void PutPoint(bool b);
	void render();

#ifndef Method2
	void OnIdle(wxIdleEvent& event);
#endif
	DECLARE_EVENT_TABLE()
};

class MyApp : public wxApp
{
public:
public:
	virtual bool OnInit();
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	MyFrame *frame = new MyFrame((wxFrame *)NULL, -1, _T("Hello World"),
		wxPoint(50, 50), wxSize(450, 340));

	frame->Show(TRUE);
	
	return TRUE;
}


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
#ifndef Method2
	EVT_IDLE(MyFrame::OnIdle)
#else
	EVT_TIMER(1, MyFrame::OnTimer)
#endif
END_EVENT_TABLE()


#ifndef Method2
void MyFrame::OnIdle(wxIdleEvent& event)
{
	render();
	event.RequestMore();
}
#else
void MyFrame::OnTimer(wxTimerEvent& event)
{
	render();
}
#endif

void MyFrame::PutPoint(bool b)
{
	wxClientDC dc(this);
	if (b)
	{
		dc.DrawPoint(10 + xOffset, 10 + yOffset);
		dc.SetBrush(wxBrush(*wxBLACK, 0));
		dc.DrawRectangle(30 + xOffset * 10, 30 + yOffset * 10, 10, 10);
	}
	if (++xOffset > 15)
	{
		++yOffset;
		xOffset = 0;
	}
}

void MyFrame::render()
{
	if (currentFrame > 255)
		currentFrame = 0;
	xOffset = 0;
	yOffset = 0;

	wxClientDC dc(this);
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
		for (int j = 0; j < 8; ++j)
		{
			for (int i = 0; i < 8; ++i)
				PutPoint( (odata[currentFrame * 32 + i * 4] & (unsigned char)(1 << j)) );
			for (int i = 0; i < 8; ++i)
				PutPoint( (odata[currentFrame * 32 + i * 4 + 1] & (unsigned char)(1 << j)));
		}
		for (int j = 0; j < 8; ++j)
		{
			for (int i = 0; i < 8; ++i)
				PutPoint( (odata[currentFrame * 32 + i * 4 + 2] & (unsigned char)(1 << j)));
			for (int i = 0; i < 8; ++i)
				PutPoint( (odata[currentFrame * 32 + i * 4 + 3] & (unsigned char)(1 << j)));
		}
#ifndef Method2
		usleep(200000);
#endif
	++currentFrame;
}