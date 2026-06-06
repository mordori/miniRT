#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

extern "C" {
#include "defines.h"
#include "rendering.h"
#include "ui.hpp"
}

static bool g_ui_dirty = false;

extern "C" bool check_ui_dirty(void) {
	bool state = g_ui_dirty;
	g_ui_dirty = false;
	return state;
}

void init_ui() {
	GLFWwindow* window = glfwGetCurrentContext();
	if (!window)
		return;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	// io.ConfigViewportsNoAutoMerge = true;
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
				if (ImGui::Button("Save render", ImVec2(150.0f, 0.0f))) {}
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

		if (ctx->editor.selected_obj != NULL) {
			ImGuiTabItemFlags tab_flags = ImGuiTabItemFlags_SetSelected;
			if (ImGui::BeginTabItem("Object", NULL, tab_flags)) {
				ImGui::Text("Name");
				if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {}
				ImGui::Spacing();
				ImGui::Spacing();
				if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {}
				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();
	}
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
