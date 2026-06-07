#include <GLFW/glfw3.h>
#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

extern "C" {
#include "defines.h"
#include "rendering.h"
#include "ui.hpp"
#include "utils.h"
}

static bool g_ui_dirty = false;

extern "C" bool ui_check_dirty(void) {
	bool state = g_ui_dirty;
	g_ui_dirty = false;
	return state;
}

extern "C" bool ui_want_mouse(void) {
	return ImGui::GetIO().WantCaptureMouse;
}

extern "C" bool ui_want_keyboard(void) {
	return ImGui::GetIO().WantCaptureKeyboard;
}

void init_ui() {
	GLFWwindow* window = glfwGetCurrentContext();
	if (!window)
		return;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
}

void render_ui(void* param) {
	t_context* ctx = (t_context*)param;
	t_renderer* r = &ctx->renderer;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	int ww, wh;
	glfwGetWindowSize(glfwGetCurrentContext(), &ww, &wh);
	ImGui::SetNextWindowPos(ImVec2(ww - 350, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(350, wh), ImGuiCond_Always);
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoNav;
	ImGui::Begin("Side Panel", NULL, window_flags);
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	if (ImGui::BeginTabBar("SideBarTabs")) {
		if (ImGui::BeginTabItem("Scene")) {
			if (ImGui::CollapsingHeader("Renderer", ImGuiTreeNodeFlags_DefaultOpen)) {
				uint32_t min_samples = 1u, max_samples = 256u;
				uint32_t min_bounces = 1u, max_bounces = 16u;
				if (ImGui::SliderScalar(
						"Samples", ImGuiDataType_U32, &r->render_samples, &min_samples, &max_samples, "%u", ImGuiSliderFlags_Logarithmic)) {
					bool was_finished = r->frame >= r->render_samples && r->mode == RENDERED;
					if (r->render_samples > 256u) {
						r->render_samples = 256u;
					} else if (was_finished && r->frame < r->render_samples) {
						--r->frame;
						blit(ctx, r, false);
						++r->frame;
					}
				}
				g_ui_dirty |= ImGui::SliderScalar("Bounces", ImGuiDataType_U32, &r->render_bounces, &min_bounces, &max_bounces);
				ImGui::Spacing();
				ImGui::Spacing();

				float progress = 0.0f;
				char progress_text[64];
				bool is_done = false;
				if (r->mode == RENDERED) {
					progress = (float)r->frame / (float)r->render_samples;
					if (progress >= 1.0f) {
						progress = 1.0f;
						is_done = true;
					}
					float elapsed_sec = (time_now() - r->render_time) / 1000.0f;
					if (is_done)
						snprintf(progress_text, sizeof(progress_text), "Done! (%.1fs)", elapsed_sec);
					else
						snprintf(progress_text, sizeof(progress_text), "Rendering: %u / %u", r->frame, r->render_samples);
				} else {
					snprintf(progress_text, sizeof(progress_text), "Preview Mode");
				}
				if (is_done)
					ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
				else if (r->mode != RENDERED)
					progress = 1.0f;
				ImGui::ProgressBar(progress, ImVec2(-1.0f, 0.0f), progress_text);
				static uint64_t save_time = 0;
				static char last_saved_file[256] = "";
				if (is_done) {
					ImGui::PopStyleColor();
					ImGui::Spacing();
					ImGui::Spacing();
					float button_width = 150.0f;
					float avail_width = ImGui::GetContentRegionAvail().x;
					float offset = (avail_width - button_width) * 0.5f;
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);
					if (ImGui::Button("Save render", ImVec2(button_width, 0.0f))) {
						screenshot(ctx, last_saved_file, sizeof(last_saved_file));
						open_image_viewer(last_saved_file);
						save_time = time_now();
					}
					if (save_time > 0 && (time_now() - save_time) < 1000) {
						ImGui::Spacing();
						const char* text = "Image saved!";
						float text_width = ImGui::CalcTextSize(text).x;
						float text_offset = (ImGui::GetContentRegionAvail().x - text_width) * 0.5f;
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + text_offset);
						ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "%s", text);
					}
				}
				ImGui::Spacing();
				ImGui::Spacing();
				if (ImGui::CollapsingHeader("Presets", ImGuiTreeNodeFlags_DefaultOpen)) {}
				ImGui::Spacing();
				ImGui::Spacing();
				if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
					g_ui_dirty |= ImGui::SliderFloat(
						"Focal Length", &ctx->scene.cam.focal_len_mm, 14.0f, 200.0f, "%.0f", ImGuiSliderFlags_Logarithmic);
					g_ui_dirty |=
						ImGui::SliderFloat("Focus Dist", &ctx->scene.cam.focus_dist, 0.1f, 100.0f, "%.1f", ImGuiSliderFlags_Logarithmic);
					g_ui_dirty |=
						ImGui::SliderFloat("F-stop", &ctx->scene.cam.f_stop, 0.95f, 128.0f, "%.02f", ImGuiSliderFlags_Logarithmic);
					g_ui_dirty |= ImGui::SliderFloat(
						"Shutter speed", &ctx->scene.cam.shutter_speed, 0.01f, 10000.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
					g_ui_dirty |= ImGui::SliderFloat("ISO", &ctx->scene.cam.iso, 100.0f, 3000.0f, "%.0f", ImGuiSliderFlags_Logarithmic);
				}
				ImGui::Spacing();
				ImGui::Spacing();
				if (ImGui::CollapsingHeader("Environment", ImGuiTreeNodeFlags_DefaultOpen)) {
					float temp = ctx->scene.cam.skydome_uv_offset.u * 360.0f;
					if (ImGui::SliderFloat("Rotate", &temp, 0.0f, 360.0f, "%.0f")) {
						ctx->scene.cam.skydome_uv_offset.u = temp / 360.0f;
						g_ui_dirty = true;
					}
				}
				ImGui::Spacing();
				ImGui::Spacing();
				if (ImGui::CollapsingHeader("Add", ImGuiTreeNodeFlags_DefaultOpen)) {}
			}
			ImGui::EndTabItem();
		}

		bool is_selecting{ ctx->editor.selected_obj != NULL };
		if (is_selecting) {
			ImGuiTabItemFlags tab_flags = ImGuiTabItemFlags_None;
			if (ctx->editor.request_ui_tab) {
				tab_flags |= ImGuiTabItemFlags_SetSelected;
				ctx->editor.request_ui_tab = false;
			}
			if (ImGui::BeginTabItem("Object", NULL, tab_flags)) {
				ImGui::Text("Name");
				if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {}
				ImGui::Spacing();
				ImGui::Spacing();
				if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {}
				ImGui::EndTabItem();
			}
		}
	}
	ImGui::EndTabBar();
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	// ImGuiIO& io = ImGui::GetIO();
	// if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
	// 	GLFWwindow* window = glfwGetCurrentContext();
	// 	ImGui::UpdatePlatformWindows();
	// 	ImGui::RenderPlatformWindowsDefault();
	// 	glfwMakeContextCurrent(window);
	// }
}

void cleanup_ui() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
