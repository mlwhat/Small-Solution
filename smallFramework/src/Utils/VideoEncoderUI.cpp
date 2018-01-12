/***************************************************************************
# Copyright (c) 2015, NVIDIA CORPORATION. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#  * Neither the name of NVIDIA CORPORATION nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***************************************************************************/
#include "Framework.h"
#include "VideoEncoderUI.h"
#include "OSSimple.h"
#include "../imgui/imgui.h"
#include "sf.h"

namespace smallFrameWork
{


	VideoEncoderUI::UniquePtr VideoEncoderUI::create(uint32_t topLeftX, uint32_t topLeftY, uint32_t width, uint32_t height, Callback startCaptureCB, Callback endCaptureCB)
	{
		return UniquePtr(new VideoEncoderUI(topLeftX, topLeftY, width, height, startCaptureCB, endCaptureCB));
	}

	VideoEncoderUI::VideoEncoderUI(uint32_t topLeftX, uint32_t topLeftY, uint32_t width, uint32_t height, Callback startCaptureCB, Callback endCaptureCB) : mStartCB(startCaptureCB), mEndCB(endCaptureCB)
	{
		mWindowDims.x = topLeftX;
		mWindowDims.y = topLeftY;
		mWindowDims.width = width;
		mWindowDims.height = height;

		mWindowDims.width = clampi(mWindowDims.width, 0, w);
		mWindowDims.height = clampi(mWindowDims.height, 0, h);

		mWindowDims.x = clampi(mWindowDims.x, 0, mWindowDims.width);
		mWindowDims.y = clampi(mWindowDims.y, 0, mWindowDims.height);


		mWindowCropped.x_s = 0;
		mWindowCropped.x_e = w;
		mWindowCropped.y_s = 0;
		mWindowCropped.y_e = h;


	}

	void VideoEncoderUI::render( )
	{
		if (mCapturing)
		{
			endCaptureUI();
		}
		else
		{
			startCaptureUI();
		}
	}

	void VideoEncoderUI::startCaptureUI( )
	{

		ImGui::Begin("Video Capture");
		

		static bool _fps = false;
		if (ImGui::Checkbox("glfwSwapInterval", &_fps))
		{
			if (_fps)
				glfwSwapInterval(1);
			else
				glfwSwapInterval(0);
		}

		static int CodecIDSelId = 4;
		std::string comboStr;
		for (const auto& v : kCodecID)
		{
			comboStr += v.label + '\0';
		}
		if (ImGui::Combo("Codec", &CodecIDSelId, comboStr.c_str()))
		{
			mCodec = (VideoEncoder::CodecID)(kCodecID[CodecIDSelId].value);
		}

		static int PixelFormatSelId = 0;
		if (ImGui::Combo("AV  PixelFormat", &PixelFormatSelId, "packed RGBA 8:8:8:8, 32bpp, BGRABGRA...\0packed BGRA 8:8:8:8, 32bpp, BGRABGRA...\0\0"))
		{
			if (PixelFormatSelId == 0)
				pixelformat = ResourceFormat::RGBA8Unorm;
			else if(PixelFormatSelId == 1)
				pixelformat = ResourceFormat::BGRA8Unorm;
		}

		if (ImGui::SliderInt("Video FPS", &mFPS, 4,60)) 
		{
			mFPS = clampi(mFPS, 0, 240);
			cout << " mVideoCapture.pUI->getFPS();" << mFPS << endl;
		}



		ImGui::SliderInt("Frame Video Start Pos X:", &mWindowCropped.x_s, 0, mWindowCropped.x_e);
		ImGui::SliderInt("Frame Video Start End X:", &mWindowCropped.x_e, mWindowCropped.x_s, w);
		ImGui::SliderInt("Frame Video End Pos Y:", &mWindowCropped.y_s, 0, mWindowCropped.y_e);
		ImGui::SliderInt("Frame Video End End Y:", &mWindowCropped.y_e, mWindowCropped.y_s, h);

		if (ImGui::TreeNode("Codec Options"))
		{
			ImGui::Unindent();
			ImGui::DragFloat("Bitrate (Mbps)", &mBitrate, 0.01f);
			mBitrate = clampf(mBitrate, 0, FLT_MAX);
			ImGui::InputInt("GOP Size", &mGopSize, 1);
			mGopSize = clampi(mGopSize, 0, 100000);

			ImGui::Indent();

			ImGui::TreePop();
		}
		
		ImGui::Checkbox("Flip Y", &mFilpY);

		if (ImGui::Checkbox("Capture UI", &mCaptureUI)) 
		{
			cout << "Capture UI : "<< mCaptureUI << endl;
		}

	
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(450.0f);
			ImGui::TextUnformatted("Check this box if you want the GUI recorded");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

//		ImGui::Checkbox("Use Time-Range", &mUseTimeRange);

		if (ImGui::Button("Start Recording"))
		{
			startCapture();
		}
		if (ImGui::Button("Cancel"))
		{
			mEndCB();
		}
		ImGui::End();
	}

	VideoEncoderUI::~VideoEncoderUI() = default;

	void VideoEncoderUI::startCapture()
	{
		if (saveFileDialog(VideoEncoder::getSupportedContainerForCodec(mCodec).c_str(), mFilename))
		{
			//if (!mUseTimeRange)
			//{
			//	mCaptureUI = true;
			//}

			mCapturing = true;

			// Call the users callback
			mStartCB();
		}
	}

	void VideoEncoderUI::endCaptureUI( )
	{
		if (mCapturing)
		{
			ImGui::Begin("Video Captureing....");
			if (ImGui::Button("End Recording"))
			{
				mEndCB();
				mCapturing = false;;
			}
			ImGui::End();
		}
	}
}