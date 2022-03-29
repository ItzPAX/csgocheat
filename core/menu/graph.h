#pragma once
#include "includes.h"
#include "pch.h"

bool bExtended = false;
void DrawAimbotGraph(float flSmoothVal, Vec2D* pBezierVals) {
	static ImPlotDragToolFlags flags = ImPlotDragToolFlags_None;
	ImPlotAxisFlags ax_flags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks;
	ImVec2 size;
	if (bExtended)
		size = { -1.f, 400.f };
	else
		size = { 250.f, 250.f };

	//if (bExtended) {
	//	ImGui::SetNextWindowSize(ImVec2(500, 600));
	//	ImGui::Begin("Aimbot Curve", nullptr);
	//	ImGui::Text("Hello");
	//	ImGui::End();
	//}

	if (ImPlot::BeginPlot("##AimbotCurve", size, ImPlotFlags_NoTitle | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMouseText | ImPlotSubplotFlags_NoResize)) {
		ImPlot::SetupAxes("Time", "Speed", ax_flags, ax_flags);
		float flMaxYVal = 1 + ((100 / flSmoothVal) / 100); // weird cell shit
		ImPlot::SetupAxesLimits(0, 1, 0, flMaxYVal, ImPlotCond_Always);
		static ImPlotPoint P[] = { ImPlotPoint(0.f,0.f), ImPlotPoint(0.25f,0.5f),  ImPlotPoint(0.75f,0.5f),  ImPlotPoint(1.f,0.f) };
		static ImPlotPoint PCached[] = { P[1], P[2] };

		if (bExtended) {
			if (ImPlot::DragPoint(1, &P[1].x, &P[1].y, ImVec4(1, 0.5f, 1, 1), 4, flags, flMaxYVal, 0)) {
				PCached[0] = P[1];
				PCached[1] = P[2];
			}
			if (ImPlot::DragPoint(2, &P[2].x, &P[2].y, ImVec4(0, 0.5f, 1, 1), 4, flags, flMaxYVal, 0)) {
				PCached[0] = P[1];
				PCached[1] = P[2];
			}
		}

		ImVec2 size = ImPlot::GetPlotSize();
		ImVec2 pos = ImPlot::GetPlotPos();

		if (g_Math.IsInRect(pos, size) && !bExtended) {
			if (GetAsyncKeyState(VK_LBUTTON) & 0x01)
				bExtended = !bExtended;
		}
		if (bExtended) {
			if (GetAsyncKeyState(VK_RBUTTON) & 0x01 || GetAsyncKeyState(VK_ESCAPE) & 0x01)
				bExtended = !bExtended;
		}

		ImPlot::SetupLegend(ImPlotLocation_South, ImPlotLegendFlags_Horizontal);

		// Bezier spline
		static ImPlotPoint B[100];
		for (int i = 0; i < 100; ++i) {
			double t = i / 99.0;
			double u = 1 - t;
			double w1 = u * u * u;
			double w2 = 3 * u * u * t;
			double w3 = 3 * u * t * t;
			double w4 = t * t * t;
			B[i] = ImPlotPoint(w1 * P[0].x + w2 * PCached[0].x + w3 * PCached[1].x + w4 * P[3].x, w1 * P[0].y + w2 * PCached[0].y + w3 * PCached[1].y + w4 * P[3].y);
			pBezierVals[i].x = B[i].x;
			pBezierVals[i].y = B[i].y;
		}

		if (bExtended) {
			ImPlot::SetNextLineStyle(ImVec4(1, 0.5f, 1, 1));
			ImPlot::PlotLine("##h1", &P[0].x, &P[0].y, 2, 0, sizeof(ImPlotPoint));
			ImPlot::SetNextLineStyle(ImVec4(0, 0.5f, 1, 1));
			ImPlot::PlotLine("##h2", &P[2].x, &P[2].y, 2, 0, sizeof(ImPlotPoint));
		}
		ImPlot::SetNextLineStyle(ImVec4(0, 0.9f, 0, 1), 2);
		ImPlot::PlotLine("##Speed", &B[0].x, &B[0].y, 100, 0, sizeof(ImPlotPoint));

		ImPlot::EndPlot();
	}
}