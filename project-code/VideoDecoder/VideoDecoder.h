//
// Created by elnsa on 2020-01-15.
//

#ifndef PROJECT_CODE_VIDEODECODER_H
#define PROJECT_CODE_VIDEODECODER_H

#include "../StreamPackets/ESPackets/ESPacket.h"
#include "../PictureDecoder/PictureDecoder.h"
#include "../StreamPackets/ESPackets/RegularStartCodes/SequenceDisplayExtensionPacket.h"
#include "../CImg283/CImg.h"

class VideoDecoder {
private:
    static VideoDecoder *instance;

    PictureDecoder *pictureDecoder;

    /**
     * Keep track of the Png Sequence
     */
    size_t pngSequenceNumber;

    /**
     * A private constructor for the singleton
     */
    VideoDecoder();

    static bool loadFile(const char *relative_path);

    void loadVideoSequence();

public:
    /**
   * Function to return a pointer to the singleton instance
   * @return VideoDecoder* instance
   */
    static VideoDecoder *getInstance() {
        if (!instance) {
            instance = new VideoDecoder();
        }
        return instance;
    }

    HPicture* makeOnePicture();

    cimg_library::CImg<int>** decodeToArray(const char* source);

    void decodeToFile(const char *source, const char *destination);

    void loadExtensionUserData(unsigned char i);

    /**
     * H262 6.3.8 NOTE – The information carried by time_code plays no part in the decoding process.
     */
    void loadGroupHeaderAndExtension();

    PictureHeaderPacket::picture_coding_types loadPictureHeader();

    void loadPictureCodingExtension();

    void handleVideoStream(ESPacket *pPacket);

    void makePicture(char *destination);

    void loadSequenceDisplayExtension(SequenceDisplayExtensionPacket *pPacket);

    ESPacket *getNextVideoPacket();

    bool nextVideoPacketIs(ESPacket::start_code startCode);

    PictureDecoder *getPictureDecoder() const;

    void savePngToFile(HPicture *hPicture, char *destination);

};


#endif //PROJECT_CODE_VIDEODECODER_H
