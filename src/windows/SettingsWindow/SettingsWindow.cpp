#include "SettingsWindow.h"
#include "common/Logger.h"
#include "common/midi/Midi.h"

SettingsWindow::SettingsWindow()
{
    LOG_DEBUG("SettingsWindow inited");
}

void SettingsWindow::draw()
{
    static const PmDeviceInfo* inputDevice = Midi::instance().getSelectedInputDevice();
    static const PmDeviceInfo* outputDevice = Midi::instance().getSelectedOutputDevice();

    ImGui::Begin("Settings", nullptr, WINDOW_FLAGS);

    ImGui::Checkbox("Enable QWERTY output", &Settings::instance().enableOutput);
    ImGui::Checkbox("Enable velocity", &Settings::instance().enableVelocity);
    ImGui::Checkbox("Enable sustain", &Settings::instance().enableSustain);

    if (ImGui::Button("MIDI input"))
        ImGui::OpenPopup("midi_input_popup");
    ImGui::SameLine();
    ImGui::TextUnformatted(inputDevice ? inputDevice->name : "<None>");
    if (ImGui::BeginPopup("midi_input_popup"))
    {
        ImGui::SeparatorText("MIDI input devices");
        if (ImGui::Selectable("<None>"))
        {
            Midi::instance().setInputDevice(pmNoDevice);
            inputDevice = nullptr;
        }
        for (const auto& [deviceId, deviceInfo] : Midi::instance().getDevices())
        {
            if (deviceInfo->input && ImGui::Selectable(deviceInfo->name))
            {
                Midi::instance().setInputDevice(deviceId);
                inputDevice = Midi::instance().getSelectedInputDevice();
            }
        }
        ImGui::EndPopup();
    }

    if (ImGui::Button("MIDI output"))
        ImGui::OpenPopup("midi_output_popup");
    ImGui::SameLine();
    ImGui::TextUnformatted(outputDevice ? outputDevice->name : "<None>");
    if (ImGui::BeginPopup("midi_output_popup"))
    {
        ImGui::SeparatorText("MIDI output devices");
        if (ImGui::Selectable("<None>"))
        {
            Midi::instance().setOutputDevice(pmNoDevice);
            outputDevice = nullptr;
        }
        for (const auto& [deviceId, deviceInfo] : Midi::instance().getDevices())
        {
            if (deviceInfo->output && ImGui::Selectable(deviceInfo->name))
            {
                Midi::instance().setOutputDevice(deviceId);
                outputDevice = Midi::instance().getSelectedOutputDevice();
            }
        }
        ImGui::EndPopup();
    }

    static std::string fileName = "<None>";
    if (ImGui::Button("Open MIDI file"))
        ImGui::OpenPopup("midi_file_popup");
    ImGui::SameLine();
    ImGui::TextUnformatted(fileName.c_str());
    if (ImGui::BeginPopup("midi_file_popup"))
    {
        ImGui::SeparatorText("Select MIDI file");
        if (ImGui::Selectable("<None>"))
        {
            Midi::instance().stopMidiFile();
            Midi::instance().closeMidiFile();
            fileName = "<None>";
        }

        for (const auto& entry : std::filesystem::directory_iterator("./songs/")) {
            if (entry.is_regular_file()) {
                std::string file = entry.path().filename().string();
                if (!file.ends_with(".mid") && !file.ends_with(".midi"))
                    continue;
                if (ImGui::Selectable(file.c_str()))
                {
                    fileName = file;
                    Midi::instance().stopMidiFile();
                    Midi::instance().closeMidiFile();
                    Midi::instance().openMidiFile(fileName);
                }
            }
        }
        ImGui::EndPopup();
    }
    // Help tooltip
    {
        ImGui::SameLine();
        ImGui::TextDisabled("(?)");
           if (ImGui::BeginItemTooltip())
           {
               ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
               ImGui::TextUnformatted("Press 'F5' to play selected file.\nPress 'Shift + F5' to stop playing");
               ImGui::PopTextWrapPos();
               ImGui::EndTooltip();
           }
    }

    ImGui::End();
}
