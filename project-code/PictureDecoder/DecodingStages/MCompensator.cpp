//
// Created by elnsa on 2020-02-13.
//

#include <PictureDecoder.h>
#include <VideoDecoder.h>
#include <VideoInformation.h>
#include "MCompensator.h"
#include "../../VideoDecoder/VideoException.cpp"

/**
 * TODO (remove these) Notes:
 * In  P-pictures  prediction  shall  be  made  from  the  most  recently  reconstructed  reference  frame.  This  is  illustrated  in  Figure 7-10.
 * [ ] 7.6
 * [X] 7.6.1
 * [X] 7.6.2
 * [X] 7.6.2.1
 * [X] 7.6.2.2
 * [X] 7.6.3
 * [X] 7.6.3.1
 * [X] 7.6.3.2
 * [X] 7.6.3.3
 * [X] 7.6.3.4
 * [X] 7.6.3.5
 * [X] 7.6.3.6
 * [X] 7.6.3.7
 * [X] 7.6.3.8
 * [X] 7.6.3.9
 * [ ] 7.6.4
 * [ ] 7.6.5
 * [ ] 7.6.6
 * [ ] 7.6.6.1
 * [ ] 7.6.6.2
 * [ ] 7.6.6.3
 * [ ] 7.6.6.4
 * [ ] 7.6.7
 * [ ] 7.6.7.1
 * [ ] 7.6.7.2
 * [ ] 7.6.7.3
 * [ ] 7.6.7.4
 * [ ] 7.6.8
 */

/**
 * Constructor
 * @param frameType: I-Picture, P-Picture or B-Picture
 * @param concealment_motion_vectors
 */
MCompensator::MCompensator(PictureHeaderPacket::picture_coding_types frameType, bool concealment_motion_vectors) {
    this->frameType = frameType;
    this->concealment_motion_vectors = concealment_motion_vectors;
    resetPMV(); // Initialize all predictors to 0
}

void MCompensator::performMComp(HPicture *picture) {
    switch (frameType) {
        case PictureHeaderPacket::picture_coding_types::intra_coded:
            printf("TODO MComp handle I-frame\n");
            setMacroblockAddresses(picture);
            //TODO handle I-frame: add to framestore + anything else?
            break;
        case PictureHeaderPacket::picture_coding_types::predictive_coded:
            performMcompPPicture(picture);
            break;
        default:
            throw VideoException(
                    "MotionCompensator::performMComp: Picture type not supported\n");
    }
}

void MCompensator::performMcompPPicture(HPicture *picture) {
    addMissingMacroblocks(picture);
    setMacroblockAddresses(picture);
    for (size_t s = 0; s < picture->getNumSlices(); s++) {
        Slice *slice = picture->getSlices()[s];
        resetPMV(); //At the start of each slice H.262 7.6.3.4
        for (size_t m = 0; m < slice->getNumMacroblocks(); m++) {
            Macroblock *macroblock = slice->getMacroblocks()[m];
            if (macroblock->getMacroBlockModes()->getFrameMotionType() != 0b10) {
                throw VideoException("MCompensator::performMcompPPicture: Prediction type not handled by decoder.");
            }
            //This code only handles Frame pictures
            performMCompMacroblock(macroblock);
            //TODO continue here once helper is done
        }
    }
}

void MCompensator::performMCompMacroblock(Macroblock *macroblock) {
    checkResetPMV(macroblock);
    decodeMotionVectors(macroblock); // Decodes motion vectors according to H.262 7.6.3.1
    updateRemainingPredictors(macroblock); // Updates predictors according to H.262 7.6.3.3
    handleMissingPredictors(macroblock); // Makes missing vectors according to H.262 7.6.3.5
    makeChromVectors(macroblock); // Builds chrominace vectors from luminance vectors according to H.262 7.6.3.7

    //TODO implement
    //TODO continue with 7.6.4 here
}

/**
 * Decodes motion vectors according to H.262 7.6.3.1
 */
void MCompensator::decodeMotionVectors(Macroblock *macroblock) {
    if (macroblock->getForwardMotionVectors()) {
        MotionVector *mv0s = macroblock->getForwardMotionVectors()->getMotionVector0S();
        if (mv0s) decodeVectorPrime(mv0s);
        MotionVector *mv1s = macroblock->getForwardMotionVectors()->getMotionVector1S();
        if (mv1s) decodeVectorPrime(mv1s);
    }
    if (macroblock->getForwardMotionVectors()) {
        MotionVector *bv0s = macroblock->getForwardMotionVectors()->getMotionVector0S();
        if (bv0s) decodeVectorPrime(bv0s);
        MotionVector *bv1s = macroblock->getForwardMotionVectors()->getMotionVector1S();
        if (bv1s) decodeVectorPrime(bv1s);
    }
}

void MCompensator::decodeVectorPrime(MotionVector *motionVector) {
    motionVector->setLumVectorRS0(decodeVectorPrimeHelper(motionVector->getMotionCodeRS0(),
                                                          motionVector->getMotionResidualRS0(),
                                                          motionVector->isR(),
                                                          motionVector->isS(),
                                                          0)); // NOLINT(modernize-use-bool-literals)
    motionVector->setLumVectorRS1(decodeVectorPrimeHelper(motionVector->getMotionCodeRS0(),
                                                          motionVector->getMotionResidualRS0(),
                                                          motionVector->isR(),
                                                          motionVector->isS(),
                                                          1)); // NOLINT(modernize-use-bool-literals)
}

int MCompensator::decodeVectorPrimeHelper(char vectorCode, unsigned char residual, bool r, bool s, bool t) {
    unsigned int r_size = (unsigned int) VideoDecoder::getInstance()->getPictureDecoder()->getFCodeST(s, t) - 1;
    int f = 1 << r_size; // NOLINT(hicpp-signed-bitwise)
    int high = (16 * f) - 1;
    int low = ((-16) * f);
    int range = (32 * f);
    int delta;
    if ((f == 1) || (vectorCode == 0)) delta = vectorCode;
    else {
        delta = ((abs(vectorCode) - 1) * f) + residual + 1;
        if (vectorCode < 0) delta = -delta;
    }
    int prediction = PMV[r][s][t];
    /**
     * Field predictions are not handled by the decoder
     * if ( (mv_format == "field") && (t==1) && (picture_structure == "Frame picture") )
     *      prediction=PMV[s][t] DIV 2;
     */
    int out = prediction + delta;
    if (out < low) out += range;
    if (out > high) out -= range;
    /**
     * Field predictions are not handled by the decoder
     * if ((mv_format == "field") && (t == 1) && (picture_structure == "Frame picture"))
     *      PMV[r][s][t] = vector'[r][s][t] * 2;
     */
    PMV[r][s][t] = out;
    return out;
}

/**
 * Update remaining predictors according to H.262 7.6.3.3
 * This decoder does not handle field pictures
 * @param macroblock
 */
void MCompensator::updateRemainingPredictors(Macroblock *macroblock) {
    bool mmf = macroblock->getMacroBlockModes()->isMacroblockMotionForward();
    bool mmb = macroblock->getMacroBlockModes()->isMacroblockMotionBackward();
    if (macroblock->getMacroBlockModes()->isMacroblockIntra()) {
        PMV[1][0][0] = PMV[0][0][0];
        PMV[1][0][1] = PMV[0][0][1];
    } else {
        if (mmf) {
            PMV[1][0][0] = PMV[0][0][0];
            PMV[1][0][1] = PMV[0][0][1];
        }
        if (mmb) {
            PMV[1][1][0] = PMV[0][1][0];
            PMV[1][1][1] = PMV[0][1][1];
        }
    }
}

/**
 * Handles the case of missing predictors according to H.262 7.6.3.5
 */
void MCompensator::handleMissingPredictors(Macroblock *macroblock) {
    if (!macroblock->getMacroBlockModes()->isMacroblockMotionForward() &&
        !macroblock->getMacroBlockModes()->isMacroblockIntra()) {
        macroblock->getMacroBlockModes()->setFrameMotionType(0b10);
        macroblock->setForwardMotionVectors(MotionVectors::buildZeroVectors(0)); // NOLINT(modernize-use-bool-literals)
    }
}

/**
 * Initialize Chrominance Vectors according to H.262 7.6.3.7
 * @param motionVectors
 */
void MCompensator::makeChromVectors(Macroblock *macroblock) {
    SequenceExtensionPacket::chroma_format_type chromaType = VideoInformation::getInstance()->getChromaFormat();
    switch (chromaType) { // NOLINT(hicpp-multiway-paths-covered)
        case SequenceExtensionPacket::chroma_format_type::cf_420:
            if (macroblock->getForwardMotionVectors()) makeChromVectors420(macroblock->getForwardMotionVectors());
            if (macroblock->getBackwardMotionVectors()) makeChromVectors420(macroblock->getBackwardMotionVectors());
            break;
            //4:2:2 and 4:4:4 cases should branch here
        default:
            throw VideoException("MCompensator::makeChromVectors: Unhandled chroma format.\n");
    }
}

/**
 * Initialize chrominance Vectors according to H.262 7.6.3.7 when chroma_format = 4:2:0
 * @param motionVectors
 */
void MCompensator::makeChromVectors420(MotionVectors *motionVectors) {
    MotionVector *mv0s = motionVectors->getMotionVector0S();
    MotionVector *mv1s = motionVectors->getMotionVector1S();
    if (mv0s) {
        mv0s->setChromVectorRS0(mv0s->getLumVectorRS0() / 2);
        mv0s->setChromVectorRS1(mv0s->getLumVectorRS1() / 2);
    }
    if (mv1s) {
        mv1s->setChromVectorRS0(mv1s->getLumVectorRS0() / 2);
        mv1s->setChromVectorRS1(mv1s->getLumVectorRS1() / 2);
    }
}

/**
 * Decides whether to reset predictors according to H.262 7.6.3.4
 */
void MCompensator::checkResetPMV(Macroblock *macroblock) {
    bool intra = macroblock->getMacroBlockModes()->isMacroblockIntra();
    bool motion_forward = macroblock->getMacroBlockModes()->isMacroblockMotionForward();
    if ((intra && !concealment_motion_vectors) || //H.262 7.6.3.4
        (!intra && !motion_forward) ||
        (macroblock->getMacroblockAddressIncrement() > 1)) {
        resetPMV();
    }
}

/**
 * Helper for checkResetPMV that resets all predictors to 0
 */
void MCompensator::resetPMV() {
    for (size_t r = 0; r < 2; r++) { // NOLINT(modernize-loop-convert)
        for (size_t s = 0; s < 2; s++) {
            for (size_t t = 0; t < 2; t++) {
                PMV[r][s][t] = 0;
            }
        }
    }
}

void MCompensator::addMissingMacroblocks(HPicture *picture) {
    for (size_t s = 0; s < picture->getNumSlices(); s++) {
        Slice *slice = picture->getSlices()[s];
        for (size_t m = 0; m < slice->getNumMacroblocks(); m++) {
            Macroblock *mb = slice->getMacroblocks()[m];
            if (mb->getMacroblockAddressIncrement() > 1) {
                for(size_t i = mb->getMacroblockAddressIncrement(); i > 1; i--){
                    slice->insertZeroVectorMacroblock(m);
                    m++;//increment past inserted macroblock
                }
            }
        }
    }
}

void MCompensator::setMacroblockAddresses(HPicture *picture) {
    size_t index = 0;
    for (size_t s = 0; s < picture->getNumSlices(); s++) {
        Slice *slice = picture->getSlices()[s];
        for (size_t m = 0; m < slice->getNumMacroblocks(); m++) {
            Macroblock *mb = slice->getMacroblocks()[m];
            mb->setMacroblockAddress(index);
            index++;
        }
    }
}