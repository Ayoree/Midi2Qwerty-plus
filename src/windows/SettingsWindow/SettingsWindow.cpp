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
    ImGui::Checkbox("Enable velocity", &Settings::instance().enableVelocity); // TODO: implement this

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

    ImGui::End();
}
