//
// Created by bhave on 11/19/2019.
//


#include <ctime>
#include "ESParser.cpp"
#include "VideoDecoder.h"


// note - global constants
// DESIGN - use builtin math


//const static double e = 2.71828182845;


/* TODO: display the information in part 1
 * DESIGN: returns an int or a metadata struct
 *         inputs
 *         display and parse separately
*/

int main() {
    clock_t t = clock();
    try {
        std::string relative_path, destination;
        std::cout << "Enter the relative path of the file to be decoded - ";
        std::cin >> relative_path;
        std::cout << "Enter the relative path of the folder to store the output frames - ";
        std::cin >> destination; //TODO clear destination folder before running the program
        VideoDecoder *vDecoder = VideoDecoder::getInstance();
        vDecoder->decodeToFile(relative_path.c_str(), destination.c_str());
    } catch (PacketException &e) {
        printf("Gracefully shutting down after %f seconds.\n%s", ((float) (clock() - t)) / CLOCKS_PER_SEC, e.what());
    } catch (VideoException &e) {
        printf("Gracefully shutting down after %f seconds.\n%s", ((float) (clock() - t)) / CLOCKS_PER_SEC, e.what());
    }
}


