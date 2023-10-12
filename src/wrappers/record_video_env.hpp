#ifndef WRAPPERS_RECORD_VIDEO_ENV_H
#define WRAPPERS_RECORD_VIDEO_ENV_H

#include <cstdio>
#include <filesystem>
#include <iostream>

#include "pretty_print.hpp"
#include "raylib.h"

#include "constants.hpp"
#include "environment.hpp"
#include "state.hpp"

class RecordVideoEnvironment: public EnvironmentBase {
  private:
    PacmanEnvironment &env;

  private:
    bool should_record;
    u32 fps;
    std::filesystem::path video_folder;
    std::filesystem::path tmp_screenshot_folder;
    std::string output_filename;
  
  public:
    RecordVideoEnvironment(
      PacmanEnvironment &env,
      bool should_record = true,
      u32 fps = 24,
      const std::string &video_folder = "recordings",
      const std::string &output_filename = "recording.mp4"
    ):
      env(env),
      should_record(should_record),
      fps(fps),
      video_folder(video_folder),
      tmp_screenshot_folder(video_folder),
      output_filename(output_filename) {
      if (env.get_render_mode() != RenderMode::human)
        throw std::runtime_error("RecordVideoEnvironment currently only supports RenderMode::human");
      tmp_screenshot_folder /= "tmp";
      if (!std::filesystem::exists(video_folder))
        std::filesystem::create_directory(video_folder);
      if (!std::filesystem::exists(tmp_screenshot_folder))
        std::filesystem::create_directory(tmp_screenshot_folder);
    }

    State reset() override {
      return env.reset();
    }

    State step(MovementDirection direction) override {
      return env.step(direction);
    }

    State get_state() const override {
      return env.get_state();
    }

    std::string get_snapshot(i32 step_index = -1) const {
      if (step_index == -1)
        step_index = env.get_state().step_index;
      std::string filename = TextFormat("%s/%08d.png", tmp_screenshot_folder.c_str(), step_index);
      if (!std::filesystem::exists(filename))
        throw std::runtime_error(TextFormat("Snapshot %s does not exist. Did you forget to call render()?", filename.c_str()));
      return filename;
    }

    void render() override {
      env.render();
      if (should_record)
        TakeScreenshot(TextFormat("%s/%08d.png", tmp_screenshot_folder.c_str(), env.get_state().step_index));
    }

    void close() const override {
      env.close();

      std::string command = TextFormat(
        "ffmpeg -y -framerate %d -i %s/%%08d.png -c:v libx264 -pix_fmt yuv420p %s/%s",
        fps,
        tmp_screenshot_folder.c_str(),
        video_folder.c_str(),
        output_filename.c_str()
      );
      std::cout << command << std::endl;
      int result = std::system(command.c_str());

      if (result != 0)
        std::cout << "Error while running ffmpeg. Make sure that it is installed and in your PATH." << std::endl;
      
      std::filesystem::remove_all(tmp_screenshot_folder);
    }

    void continue_recording() {
      should_record = true;
    }

    void pause_recording() {
      should_record = false;
    }

    const PacmanEnvironment &get_env() const {
      return env;
    }
};

#endif // WRAPPERS_RECORD_VIDEO_ENV_H
