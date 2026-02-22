// simple_lp.cpp
// Compile: g++ -O3 -std=c++17 simple_lp.cpp -lsndfile -o simple_lp
// Usage: ./simple_lp in.wav out.wav [cutoff_hz]
// Default cutoff_hz = 5000.0

#include <sndfile.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <string>

int main(int argc, char* argv[]){
    if(argc < 3){
        std::cerr<<"Usage: "<<argv[0]<<" in.wav out.wav [cutoff_hz]\n";
        return 1;
    }
    std::string inpath = argv[1];
    std::string outpath = argv[2];
    double fc = (argc >= 4) ? atof(argv[3]) : 5000.0; // cutoff in Hz

    SF_INFO sfinfo;
    SNDFILE* infile = sf_open(inpath.c_str(), SFM_READ, &sfinfo);
    if(!infile){ std::cerr<<"Error opening input: "<< sf_strerror(NULL) <<"\n"; return 1; }

    int channels = sfinfo.channels;
    int sr = sfinfo.samplerate;
    sf_count_t frames = sfinfo.frames;

    std::vector<double> buffer(frames * channels);
    sf_count_t r = sf_readf_double(infile, buffer.data(), frames);
    sf_close(infile);
    if(r != frames) std::cerr<<"Warning: read "<<r<<" of "<<frames<<" frames\n";

    // alpha for simple one-pole low-pass: alpha = 1 - exp(-2*pi*fc / fs)
    double alpha = 1.0 - exp(-2.0 * M_PI * fc / double(sr));
    std::vector<double> prev(channels, 0.0);
    std::vector<double> out(buffer.size());

    for(sf_count_t i=0;i<frames;++i){
        for(int ch=0; ch<channels; ++ch){
            double x = buffer[i*channels + ch];
            double y = prev[ch] + alpha * (x - prev[ch]); // exponential smoothing
            out[i*channels + ch] = y;
            prev[ch] = y;
        }
    }

    // simple normalize to avoid clipping
    double maxv = 0.0;
    for(size_t i=0;i<out.size();++i) if(fabs(out[i])>maxv) maxv = fabs(out[i]);
    if(maxv > 0.999){
        double norm = 0.99 / maxv;
        for(size_t i=0;i<out.size();++i) out[i] *= norm;
        std::cerr<<"Normalized by "<<norm<<"\n";
    }

    SNDFILE* outfile = sf_open(outpath.c_str(), SFM_WRITE, &sfinfo);
    if(!outfile){ std::cerr<<"Error opening output: "<< sf_strerror(NULL) <<"\n"; return 1; }
    sf_writef_double(outfile, out.data(), frames);
    sf_close(outfile);

    std::cout<<"Processed "<<inpath<<" -> "<<outpath<<" (fc="<<fc<<" Hz)\n";
    return 0;
}
