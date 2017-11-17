#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#pragma comment(lib, "user32.lib")

#pragma once

namespace Remote_Host__Proof_of_Concept_ {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Net::Sockets;
	using namespace System::Text;
	using namespace System::Runtime::Serialization::Formatters::Binary;
	using namespace System::Runtime::Remoting::Messaging;
	using namespace System::Windows::Media::Imaging;
	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker2;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->backgroundWorker2 = (gcnew System::ComponentModel::BackgroundWorker());
			this->SuspendLayout();
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->WorkerSupportsCancellation = true;
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MyForm::backgroundWorker1_DoWork);
			// 
			// backgroundWorker2
			// 
			this->backgroundWorker2->WorkerSupportsCancellation = true;
			this->backgroundWorker2->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MyForm::backgroundWorker2_DoWork);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(600, 366);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private:
		TcpClient^ client;

		System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) 
		{
			//Send data
			backgroundWorker1->RunWorkerAsync();
		}

	private: 
		Image^ getScreenshotData()
		{
			Bitmap^ bmp = gcnew Bitmap(Screen::PrimaryScreen->Bounds.Width, Screen::PrimaryScreen->Bounds.Height);
			Graphics^ gr = Graphics::FromImage(bmp);
			gr->CopyFromScreen(0, 0, 0, 0, bmp->Size);
			return bmp;
		}

		System::Void backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) 
		{
			TcpListener^ listener = gcnew TcpListener(System::Net::IPAddress::Parse("0.0.0.0"), 8080);
			listener->Start();
			client = listener->AcceptTcpClient();
			listener->Stop();

			BinaryFormatter^ bf = gcnew BinaryFormatter();
			System::Drawing::Rectangle^ r = Screen::PrimaryScreen->Bounds;
			bf->Serialize(client->GetStream(), r);

			//Recieve data
			backgroundWorker2->RunWorkerAsync();

			while (client)
			{
				bf->Serialize(client->GetStream(), getScreenshotData());
				System::Threading::Thread::Sleep(100);
			}
		}

	private: 
		System::Void backgroundWorker2_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) 
		{
			BinaryFormatter^ bf = gcnew BinaryFormatter();
			Object^ obj;
			while (client)
			{
				obj = bf->Deserialize(client->GetStream());

				if (obj->GetType() == ::MouseMove::typeid)
				{ 
					Cursor->Position = Point(((::MouseMove^)obj)->GetX(), ((::MouseMove^)obj)->GetY());
				}

				if (obj->GetType() == ::Message::typeid)
				{
					::Message ^ m = (::Message^)obj;
					switch (((::Message^)obj)->GetMessage())
					{
					case ::Message::MsgType::MSG_TYPE_NONE:
						break;

					case ::Message::MsgType::MSG_TYPE_PING:
						break;

					case ::Message::MsgType::MSG_TYPE_LBDOWN:
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						break;

					case ::Message::MsgType::MSG_TYPE_LBUP:
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						break;

					case ::Message::MsgType::MSG_TYPE_RBDOWN:
						mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
						break;

					case ::Message::MsgType::MSG_TYPE_RBUP:
						mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
						break;

					case ::Message::MsgType::MSG_TYPE_DOUBLECLICK:
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						break;

					case ::Message::MsgType::MSG_TYPE_KBDOWN:
						keybd_event(m->GetData()[0], m->GetData()[1], m->GetData()[2], m->GetData()[3]);
						break;

					case ::Message::MsgType::MSG_TYPE_KBUP:
						keybd_event(m->GetData()[0], m->GetData()[1], m->GetData()[2], m->GetData()[3]);
						break;
					}
				}
			}

			backgroundWorker2->CancelAsync();
		}
	};
}
