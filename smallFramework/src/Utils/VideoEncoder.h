#pragma once
#include <string>
#include <memory>
#include "Formats.h"
struct AVFormatContext;
struct AVStream;
struct AVFrame;
struct SwsContext;
struct AVCodecContext;


namespace smallFrameWork
{
	class VideoEncoder
	{
	public:
		using UniquePtr = std::unique_ptr<VideoEncoder>;
		using UniqueConstPtr = std::unique_ptr<const VideoEncoder>;

		enum class CodecID : int32_t
		{
			RawVideo,
			H264,
			HEVC,
			MPEG2,
			MPEG4,
		};

		struct Desc
		{
			uint32_t fps = 60;
			uint32_t width = 0;
			uint32_t height = 0;
			float bitrateMbps = 4;
			uint32_t gopSize = 10;
			CodecID codec = CodecID::RawVideo;
			ResourceFormat format = ResourceFormat::BGRA8UnormSrgb;
			bool flipY = false;
			std::string filename;
		};

		~VideoEncoder();

		static UniquePtr create(const Desc& desc);
		void appendFrame(const void* pData);
		void endCapture();

		static const std::string getSupportedContainerForCodec(CodecID codec);
	private:
		VideoEncoder(const std::string& filename);
		bool init(const Desc& desc);

		AVFormatContext* mpOutputContext = nullptr;
		AVStream*        mpOutputStream = nullptr;
		AVFrame*         mpFrame = nullptr;
		SwsContext*      mpSwsContext = nullptr;
		AVCodecContext*  mpCodecContext = nullptr;

		const std::string mFilename;
		ResourceFormat mFormat;
		uint32_t mRowPitch = 0;
		uint8_t* mpFlippedImage = nullptr; // Used in case the image memory layout if bottom->top


	};
}