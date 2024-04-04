#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Window.H>
#include <iostream>
#include "P4CK.hpp"

void ExtractCallback(Fl_Widget* widget, void* userdata)
{
    P4CK* p4ck = static_cast<P4CK*>(userdata);

    const char* Filename = fl_file_chooser("Select PAK File", "*.pak", nullptr);
    if (!Filename)
        return;

    std::ifstream PAK(Filename, std::ios::binary);
    if (!PAK.is_open())
    {
        std::cerr << "Error while opening " << Filename << '\n';
        return;
    }

    p4ck->GetMagicBytes(PAK);
    if (!p4ck->IsValidP4CK())
    {
        fl_message_title("Error");
        fl_alert("The selected file is not a valid PAK file (bad signature).");
        return;
    }
    
    std::cout << "PAK is valid!!\n";
    p4ck->DumpFile(PAK);
}

int main()
{
    Fl_Window* window = new Fl_Window(400, 200, "PAK Extractor");
    P4CK CPak{};
    Fl_Button* ExtractButton = new Fl_Button(150, 100, 100, 40, "Extract");
    ExtractButton->callback(ExtractCallback, &CPak);
    window->end();
    window->show();
    return Fl::run();
}