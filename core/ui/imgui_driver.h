/*
	Copyright 2021 flyinghead

	This file is part of Flycast.

    Flycast is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    Flycast is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Flycast.  If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once
#include "imgui.h"
#include "gui.h"
#include <memory>
#include <unordered_map>
#include <array>

class ImGuiDriver
{
public:
	ImGuiDriver() {
		gui_initFonts();
	}
	virtual ~ImGuiDriver() = default;

	virtual void reset() {
		aspectRatios.clear();
	}

	virtual void newFrame() = 0;
	virtual void renderDrawData(ImDrawData* drawData, bool gui_open) = 0;

	virtual void present() = 0;
	virtual void setFrameRendered() {}

	virtual ImTextureID getTexture(const std::string& name) = 0;
	virtual ImTextureID updateTexture(const std::string& name, const u8 *data, int width, int height, bool nearestSampling) = 0;
	virtual void deleteTexture(const std::string& name) = 0;

	ImTextureID updateTextureAndAspectRatio(const std::string& name, const u8 *data, int width, int height, bool nearestSampling)
	{
		ImTextureID textureId = updateTexture(name, data, width, height, nearestSampling);
		if (textureId != ImTextureID())
			aspectRatios[textureId] = (float)width / height;
		return textureId;
	}

	float getAspectRatio(ImTextureID textureId)
	{
		auto it = aspectRatios.find(textureId);
		if (it != aspectRatios.end())
			return it->second;
		else
			return 1.f;
	}
	void updateAspectRatio(ImTextureID textureId, float aspectRatio) {
		if (textureId != ImTextureID())
			aspectRatios[textureId] = aspectRatio;
	}

private:
	std::unordered_map<ImTextureID, float> aspectRatios;	// TODO move this out
};

extern std::unique_ptr<ImGuiDriver> imguiDriver;

