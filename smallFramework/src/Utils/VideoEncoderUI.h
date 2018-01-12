#pragma once
#include "VideoEncoder.h"
#include <functional>
#include <map>
#include "AABB.h"
namespace smallFrameWork
{

	struct DropdownValue
	{
		int32_t value;      ///< User defined index. Should be unique between different options.
		std::string label;  ///< Label of the dropdown option.
	};

	static const std::vector <DropdownValue> kCodecID =
	{
		{ (int32_t)VideoEncoder::CodecID::RawVideo, std::string("Uncompressed") },
		{ (int32_t)VideoEncoder::CodecID::H264, std::string("H.264") },
		{ (int32_t)VideoEncoder::CodecID::HEVC, std::string("HEVC(H.265)") },
		{ (int32_t)VideoEncoder::CodecID::MPEG2, std::string("MPEG2") },
		{ (int32_t)VideoEncoder::CodecID::MPEG4, std::string("MPEG4") }
	};

	class Gui 
	{
	public:
		struct ComboData
		{
			uint32_t lastVal = -1;
			int32_t currentItem = -1;
		};
		static std::map<std::string, ComboData> mDropDownValues;
		static void pushWindowpushWindow(const char label[], uint32_t width = 0, uint32_t height = 0, uint32_t x = 0, uint32_t y = 0, bool showTitleBar = true)
		{
			//ImVec2 pos{ float(x), float(y) };
			//ImVec2 size{ float(width), float(height) };
			//ImGui::SetNextWindowSize(size, ImGuiSetCond_FirstUseEver);
			//ImGui::SetNextWindowPos(pos, ImGuiSetCond_FirstUseEver);
			//int flags = 0;
			//if (!showTitleBar)
			//{
			//	flags |= ImGuiWindowFlags_NoTitleBar;
			//}
			//ImGui::Begin(label, nullptr, flags);
		}
		static bool addDropdown(const char label[], const std::vector <DropdownValue>& values, uint32_t& var, bool sameLine = false)
		{
			//if (sameLine) ImGui::SameLine();
			//// Check if we need to update the currentItem
			//const auto& iter = mDropDownValues.find(label);
			//int curItem;
			//if ((iter == mDropDownValues.end()) || (iter->second.lastVal != var))
			//{
			//	// Search the current val
			//	for (uint32_t i = 0; i < values.size(); i++)
			//	{
			//		if (values[i].value == var)
			//		{
			//			curItem = i;
			//			mDropDownValues[label].currentItem = i;
			//			break;
			//		}
			//	}
			//}
			//else
			//{
			//	curItem = mDropDownValues[label].currentItem;
			//}

			//std::string comboStr;
			//for (const auto& v : values)
			//{
			//	comboStr += v.label + '\0';
			//}
			//comboStr += '\0';
			//uint32_t prevItem = curItem;
			////This returns true if the combo is interacted with at all
			//bool b = ImGui::Combo(label, &curItem, comboStr.c_str());
			//mDropDownValues[label].currentItem = curItem;
			//mDropDownValues[label].lastVal = values[curItem].value;
			//var = values[curItem].value;
			////Only return true if value is changed
			//return b && prevItem != curItem;
		}
	};

	class VideoEncoderUI
	{
	public:

		struct Bounding2D
		{
			int x_s, y_s, x_e, y_e;
		} mWindowCropped;
		using UniquePtr = std::unique_ptr<VideoEncoderUI>;
		using UniqueConstPtr = std::unique_ptr<const VideoEncoderUI>;
		using Callback = std::function<void(void)>;

		static UniquePtr create(uint32_t topLeftX, uint32_t topLeftY, uint32_t width, uint32_t height, Callback startCaptureCB, Callback endCaptureCB);
		~VideoEncoderUI();

		void render( );

		VideoEncoder::CodecID getCodec() const { return mCodec; }
		uint32_t getFPS() const { return mFPS; }
		bool useTimeRange() const { return mUseTimeRange; }
		bool captureUI() const { return mCaptureUI; }
		float getStartTime() const { return mStartTime; }
		float getEndTime() const { return mEndTime; }
		const std::string& getFilename() const { return mFilename; }
		float getBitrate() const { return mBitrate; }
		ResourceFormat getPixelFormat() { return pixelformat; }
		uint32_t getGopSize() const { return mGopSize; }
		bool mFilpY = true;
		void setFrameWidthAndHeight(int width, int height) 
		{
			w = width;
			h = height;

			mWindowCropped.x_s = 0;
			mWindowCropped.x_e = w;
			mWindowCropped.y_s = 0;
			mWindowCropped.y_e = h;

		}

		Bounding2D getBounding() const
		{
			return mWindowCropped;
		}


	private:
		VideoEncoderUI(uint32_t topLeftX, uint32_t topLeftY, uint32_t width, uint32_t height, Callback startCaptureCB, Callback endCaptureCB);

		void startCapture();
		void startCaptureUI( );
		void endCaptureUI( );

		bool mCapturing = false;
		Callback mStartCB = nullptr;
		Callback mEndCB = nullptr;

		int mFPS = 60;
		VideoEncoder::CodecID mCodec = VideoEncoder::CodecID::MPEG4;
		

		int w = 2400;
		int h  =2400;
		//glm::ivec4 BoundingBox = ivec4(0);

		bool mUseTimeRange = false;
		bool mCaptureUI = false;
		
		float mStartTime = 0;
		float mEndTime = FLT_MAX;
		struct
		{
			uint32_t x, y, width, height;
		} mWindowDims;



		std::string mFilename;
		float mBitrate = 4;
		int mGopSize = 10;
		ResourceFormat  pixelformat = ResourceFormat::RGBA8Unorm; //  RGBA8Unorm  AV_PIX_FMT_RGBA,      ///< packed RGBA 8:8:8:8, 32bpp, RGBARGBA...
								//  BGRA8Unorm   AV_PIX_FMT_BGRA,      ///< packed BGRA 8:8:8:8, 32bpp, BGRABGRA...
	};
}