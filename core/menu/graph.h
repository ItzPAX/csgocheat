#pragma once
#include "includes.h"
#include "pch.h"

void DrawGraph(const char* graphname, const char* xname, const char* yname, bool& extended, Vec2D* pBezierVals) {
	static ImPlotDragToolFlags flags = ImPlotDragToolFlags_None;
	ImPlotAxisFlags ax_flags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks;

	ImVec2 size;
	if (extended)
		size = { -1.f,-1.f };
	else
		size = { 250.f, 250.f };

	if (ImPlot::BeginPlot(graphname, size, ImPlotFlags_NoTitle | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMouseText | ImPlotSubplotFlags_NoResize)) {
		ImPlot::SetupAxes(xname, yname, ax_flags, ax_flags);

		ImPlot::SetupAxesLimits(0, 1, 0, 1, ImPlotCond_Always);
		static ImPlotPoint P[] = { ImPlotPoint(0.f,0.f), ImPlotPoint(0.25f,0.5f),  ImPlotPoint(0.75f,0.5f),  ImPlotPoint(1.f,0.f) };
		static ImPlotPoint PCached[] = { P[0], P[1], P[2], P[3] };

		if (extended) {
			if (ImPlot::DragPoint(1, &P[0].x, &P[0].y, ImVec4(0.1f, 0.9f, 0.1f, 1), 4, flags, 1, 0, true, 0)) {
				PCached[0] = P[0];
			}
			if (ImPlot::DragPoint(2, &P[1].x, &P[1].y, ImVec4(1, 0.5f, 1, 1), 4, flags, 1, 0)) {
				PCached[1] = P[1];
			}
			if (ImPlot::DragPoint(3, &P[2].x, &P[2].y, ImVec4(0, 0.5f, 1, 1), 4, flags, 1, 0)) {
				PCached[2] = P[2];
			}
			if (ImPlot::DragPoint(4, &P[3].x, &P[3].y, ImVec4(0.1f, 0.9f, 0.1f, 1), 4, flags, 1, 0, true, 1)) {
				PCached[3] = P[3];
			}
		}

		ImVec2 size = ImPlot::GetPlotSize();
		ImVec2 pos = ImPlot::GetPlotPos();

		if (g_Math.IsInRect(pos, size) && !extended) {
			if (GetAsyncKeyState(VK_LBUTTON) & 0x01)
				extended = !extended;
		}
		if (extended) {
			if (GetAsyncKeyState(VK_RBUTTON) & 0x01 || GetAsyncKeyState(VK_ESCAPE) & 0x01)
				extended = !extended;
		}

		// Bezier spline
		static ImPlotPoint B[100];
		for (int i = 0; i < 100; ++i) {
			double t = i / 99.0;
			double u = 1 - t;
			double w1 = u * u * u;
			double w2 = 3 * u * u * t;
			double w3 = 3 * u * t * t;
			double w4 = t * t * t;
			B[i] = ImPlotPoint(w1 * PCached[0].x + w2 * PCached[1].x + w3 * PCached[2].x + w4 * PCached[3].x, w1 * PCached[0].y + w2 * PCached[1].y + w3 * PCached[2].y + w4 * PCached[3].y);
			pBezierVals[i].x = B[i].x;
			pBezierVals[i].y = B[i].y;
		}

		if (extended) {
			ImPlot::SetNextLineStyle(ImVec4(1, 0.5f, 1, 1));
			ImPlot::PlotLine("##h1", &P[0].x, &P[0].y, 2, 0, sizeof(ImPlotPoint));
			ImPlot::SetNextLineStyle(ImVec4(0, 0.5f, 1, 1));
			ImPlot::PlotLine("##h2", &P[2].x, &P[2].y, 2, 0, sizeof(ImPlotPoint));
		}
		ImPlot::SetNextLineStyle(ImVec4(0, 0.9f, 0, 1), 2);
		ImPlot::PlotLine("##graph", &B[0].x, &B[0].y, 100, 0, sizeof(ImPlotPoint));

		ImPlot::EndPlot();
	}
}

ImVec2 PopupSize = { 500,600 };
void DrawExtendableGraph(const char* windowName, const char* xname, const char* yname, const char* graphname, bool& bextended, Vec2D* pBezierVals) {
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.06f, 0.05f, 0.07f, 1.00f));
	if (bextended) {
		ImGui::SetNextWindowSize(PopupSize, ImGuiCond_Once);
		ImGui::Begin(windowName, nullptr);
		DrawGraph(graphname, xname, yname, bextended, pBezierVals);
		ImGui::End();
	}
	else {
		DrawGraph(graphname, xname, yname, bextended, pBezierVals);
	}
	ImGui::PopStyleColor();
}