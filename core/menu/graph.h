#pragma once
#include "includes.h"
#include "pch.h"

void UpdateGraph(ImPlotPoint* p, float* cfgp, int size) {
	for (int i = 0; i < size; i++) {

	}
}

void DrawGraph(const char* graphname, const char* xname, const char* yname, bool& extended, Vec2D* pBezierVals, double* points) {
	static ImPlotDragToolFlags flags = ImPlotDragToolFlags_None;
	ImPlotAxisFlags ax_flags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks;

	ImVec2 size;
	if (extended)
		size = { -1.f,-1.f };
	else
		size = { 250.f, 250.f };

	if (ImPlot::BeginPlot(graphname, size, ImPlotFlags_NoTitle | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMouseText | ImPlotSubplotFlags_NoResize)) {
		ImPlot::SetupAxes(xname, yname, ax_flags, ax_flags);

		ImPlot::SetupAxesLimits(0, 1, 0, 2, ImPlotCond_Always);
		if (extended) {
			ImPlot::DragPoint(1, &points[0], &points[1], ImVec4(0.1f, 0.9f, 0.1f, 1), 4, flags, 2, 0, true, 0);
			ImPlot::DragPoint(2, &points[2], &points[3], ImVec4(1, 0.5f, 1, 1), 4, flags, 2, 0);
			ImPlot::DragPoint(3, &points[4], &points[5], ImVec4(0, 0.5f, 1, 1), 4, flags, 2, 0);
			ImPlot::DragPoint(4, &points[6], &points[7], ImVec4(0.1f, 0.9f, 0.1f, 1), 4, flags, 2, 0, true, 1);
		}

		ImVec2 size = ImPlot::GetPlotSize();
		ImVec2 pos = ImPlot::GetPlotPos();

		static bool bPressed = false;
		if (GetAsyncKeyState(VK_LBUTTON) != 0)
			bPressed = true;
		else if (bPressed) {
			bPressed = false;
			if (g_Math.IsInRect(pos, size) && !extended)
				extended = true;
		}
		if (extended) {
			if (GetAsyncKeyState(VK_RBUTTON) & 0x01 || GetAsyncKeyState(VK_ESCAPE) & 0x01)
				extended = false;
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
			B[i] = ImPlotPoint(w1 * points[0] + w2 * points[2] + w3 * points[4] + w4 * points[6], w1 * points[1] + w2 * points[3] + w3 * points[5] + w4 * points[7]);
			pBezierVals[i].x = B[i].x;
			pBezierVals[i].y = B[i].y;
		}

		if (extended) {
			ImPlot::SetNextLineStyle(ImVec4(1, 0.5f, 1, 1));
			ImPlot::PlotLine("##h1", &points[0], &points[1], 2, 0, sizeof(ImPlotPoint));
			ImPlot::SetNextLineStyle(ImVec4(0, 0.5f, 1, 1));
			ImPlot::PlotLine("##h2", &points[4], &points[5], 2, 0, sizeof(ImPlotPoint));
		}
		ImPlot::SetNextLineStyle(ImVec4(0, 0.9f, 0, 1), 2);
		ImPlot::PlotLine("##graph", &B[0].x, &B[0].y, 100, 0, sizeof(ImPlotPoint));

		ImPlot::EndPlot();
	}
}

ImVec2 PopupSize = { 500,600 };
void DrawExtendableGraph(const char* windowName, const char* xname, const char* yname, const char* graphname, bool& bextended, Vec2D* pBezierVals, double* points) {
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.06f, 0.05f, 0.07f, 1.00f));
	
	if (bextended) {
		ImGui::SetNextWindowSize(PopupSize, ImGuiCond_Once);
		ImGui::Begin(windowName, &bextended);
		DrawGraph(graphname, xname, yname, bextended, pBezierVals, points);
		ImGui::End();
	}
	else {
		DrawGraph(graphname, xname, yname, bextended, pBezierVals, points);
	}
	ImGui::PopStyleColor();
}