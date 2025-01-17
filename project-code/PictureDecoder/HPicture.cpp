//
// Created by elnsa on 2020-01-28.
//

#include "HPicture.h"

HPicture::HPicture() {
    state = decoding_state::variable_length_decoded;
    numSlices = 0;
    slices = nullptr;
    //TODO continue initializing stuff
}

void HPicture::addSlice(Slice *slice) {
    slices = (Slice **) realloc(slices, (sizeof(Slice *) * (numSlices + 1))); // NOLINT(bugprone-sizeof-expression)
    slices[numSlices++] =  slice;
}

HPicture::decoding_state HPicture::getState() const {
    return state;
}

void HPicture::setState(HPicture::decoding_state s) {
    HPicture::state = s;
}

size_t HPicture::getNumSlices() const {
    return numSlices;
}

void HPicture::setNumSlices(size_t numSlices) {
    HPicture::numSlices = numSlices;
}

Slice **HPicture::getSlices() const {
    return slices;
}

void HPicture::setSlices(Slice **slices) {
    HPicture::slices = slices;
}
