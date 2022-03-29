#pragma once
#include "includes.h"
#include "pch.h"

void DrawAimbotGraph() {
	static ImPlotDragToolFlags flags = ImPlotDragToolFlags_None;
	ImPlotAxisFlags ax_flags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks;
	if (ImPlot::BeginPlot("##AimbotCurve", ImVec2(-1.f, 0.f), ImPlotFlags_CanvasOnly | ImPlotSubplotFlags_NoResize)) {
		ImPlot::SetupAxes("Speed", "Time", ax_flags, ax_flags);
		ImPlot::SetupAxesLimits(0, 1, 0, 1);
		static ImPlotPoint P[] = { ImPlotPoint(0.f,0.f), ImPlotPoint(0.25f,0.5f),  ImPlotPoint(0.75f,0.5f),  ImPlotPoint(1.f,0.f) };

		ImPlot::DragPoint(1, &P[1].x, &P[1].y, ImVec4(1, 0.5f, 1, 1), 4, flags, 0.9, 0);
		ImPlot::DragPoint(2, &P[2].x, &P[2].y, ImVec4(0, 0.5f, 1, 1), 4, flags, 0.9, 0);

		// Bezier spline
		static ImPlotPoint B[100];
		for (int i = 0; i < 100; ++i) {
			double t = i / 99.0;
			double u = 1 - t;
			double w1 = u * u * u;
			double w2 = 3 * u * u * t;
			double w3 = 3 * u * t * t;
			double w4 = t * t * t;
			B[i] = ImPlotPoint(w1 * P[0].x + w2 * P[1].x + w3 * P[2].x + w4 * P[3].x, w1 * P[0].y + w2 * P[1].y + w3 * P[2].y + w4 * P[3].y);
		}


		ImPlot::SetNextLineStyle(ImVec4(1, 0.5f, 1, 1));
		ImPlot::PlotLine("##h1", &P[0].x, &P[0].y, 2, 0, sizeof(ImPlotPoint));
		ImPlot::SetNextLineStyle(ImVec4(0, 0.5f, 1, 1));
		ImPlot::PlotLine("##h2", &P[2].x, &P[2].y, 2, 0, sizeof(ImPlotPoint));
		ImPlot::SetNextLineStyle(ImVec4(0, 0.9f, 0, 1), 2);
		ImPlot::PlotLine("##bez", &B[0].x, &B[0].y, 100, 0, sizeof(ImPlotPoint));

		ImPlot::EndPlot();
	}
}