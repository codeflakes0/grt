/*
GRT MIT License
Copyright (c) <2012> <Nicholas Gillian, Media Lab, MIT>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#define GRT_DLL_EXPORTS
#include "LocalPeakDetection.h"
#include <iomanip>

GRT_BEGIN_NAMESPACE

//Define the string that will be used to identify the object
std::string LocalPeakDetection::id = "LocalPeakDetection";
std::string LocalPeakDetection::getId() { return LocalPeakDetection::id; }

//Register the LocalPeakDetection module with the FeatureExtraction base class
RegisterFeatureExtractionModule< LocalPeakDetection > LocalPeakDetection::registerModule( LocalPeakDetection::getId() );

LocalPeakDetection::LocalPeakDetection(const UINT numDimensions/*=1*/, const Float minDelta/*=0.5*/,const ULONG bufferLength/*=100*/) : FeatureExtraction( LocalPeakDetection::getId() )
{
    infoLog << "LocalPeakDetection::constructor d=" << numDimensions << " delta=" << minDelta << " bufferLength=" << bufferLength << std::endl;
    init(numDimensions, minDelta, bufferLength);
}

LocalPeakDetection::LocalPeakDetection(const LocalPeakDetection &rhs) : FeatureExtraction( LocalPeakDetection::getId() )
{
    infoLog << "LocalPeakDetection::copy constructor" << std::endl;
    //Invoke the equals operator to copy the data from the rhs instance to this instance
    *this = rhs;
}

LocalPeakDetection::~LocalPeakDetection(){
    
}

LocalPeakDetection& LocalPeakDetection::operator=(const LocalPeakDetection &rhs){
    if(this!=&rhs){
        this->minDelta = rhs.minDelta;
        this->bufferLength = rhs.bufferLength;
        this->currentPosition = rhs.currentPosition;

        this->minPerDimension = rhs.minPerDimension;
        this->maxPerDimension = rhs.maxPerDimension;
        this->minPosPerDimension = rhs.minPosPerDimension;
        this->maxPosPerDimension = rhs.maxPosPerDimension;
        this->lookForMaxPerDimension= rhs.lookForMaxPerDimension;

        this->dataBuffer = rhs.dataBuffer;

        copyBaseVariables( (FeatureExtraction*)&rhs );
    }
    return *this;
}

bool LocalPeakDetection::deepCopyFrom(const FeatureExtraction *featureExtraction){
    
    if( featureExtraction == NULL ) return false;
    
    if( this->getId() == featureExtraction->getId() ){
        //Invoke the equals operator to copy the data from the rhs instance to this instance
        *this = *dynamic_cast<const LocalPeakDetection*>(featureExtraction);
        return true;
    }
    
    errorLog << "deepCopyFrom(FeatureExtraction *featureExtraction) -  FeatureExtraction Types Do Not Match!" << std::endl;
    
    return false;
}

bool LocalPeakDetection::computeFeatures(const VectorFloat &inputVector){
    
    if( !initialized ){
        errorLog << "computeFeatures(const VectorFloat &inputVector) - Not initialized!" << std::endl;
        return false;
    }
    
    if( inputVector.getSize() != numInputDimensions ){
        errorLog << "computeFeatures(const VectorFloat &inputVector) - The size of the inputVector (" << inputVector.getSize() << ") does not match that of the filter (" << numInputDimensions << ")!" << std::endl;
        return false;
    }
    
    update( inputVector );
    
    return true;
}

bool LocalPeakDetection::reset(){
    if( initialized ){
        infoLog << "reset()" << std::endl;
        return init(numInputDimensions, minDelta, bufferLength);
    }
    return false;
}

bool LocalPeakDetection::save( std::fstream &file ) const{
    
    if( !file.is_open() ){
        errorLog << "save(fstream &file) - The file is not open!" << std::endl;
        return false;
    }
    
    //Write the file header
    file << "GRT_PEAK_DETECTION_FILE_V1.0" << std::endl;
    
    //Save the base settings to the file
    if( !saveFeatureExtractionSettingsToFile( file ) ){
        errorLog << "saveFeatureExtractionSettingsToFile(fstream &file) - Failed to save base feature extraction settings to file!" << std::endl;
        return false;
    }

    //Write the zero crossing counter settings
    file << "minDelta: " << minDelta << std::endl;
    file << "bufferLength: " << bufferLength << std::endl;
    
    return true;
}

bool LocalPeakDetection::load( std::fstream &file ){
    
    if( !file.is_open() ){
        errorLog << "load(fstream &file) - The file is not open!" << std::endl;
        return false;
    }
    
    std::string word;
    
    //Load the header
    file >> word;
    
    if( word != "GRT_PEAK_DETECTION_FILE_V1.0" ){
        errorLog << "load(fstream &file) - Invalid file format!" << std::endl;
        return false;
    }
    
    if( !loadFeatureExtractionSettingsFromFile( file ) ){
        errorLog << "loadFeatureExtractionSettingsFromFile(fstream &file) - Failed to load base feature extraction settings from file!" << std::endl;
        return false;
    }

    file >> word;
    if( word != "minDelta:" ){
        errorLog << "load(fstream &file) - Failed to read minDelta header!" << std::endl;
        return false;
    }
    file >> minDelta;

    file >> word;
    if( word != "bufferLength:" ){
        errorLog << "load(fstream &file) - Failed to read bufferLength header!" << std::endl;
        return false;
    }
    file >> bufferLength;
    
    //Init the LocalPeakDetection module to ensure everything is initialized correctly
    return init(numInputDimensions, minDelta, bufferLength);
}

bool LocalPeakDetection::init(const UINT numDimensions, const Float minDelta, const ULONG bufferLength){
    
    initialized = false;
    
    if( bufferLength == 0 ){
        errorLog << "init(...) - The number of bufferLength must be greater than zero!" << std::endl;
        return false;
    }

    if( numDimensions == 0 ){
        errorLog << "init(...) - The number of dimensions must be greater than zero!" << std::endl;
        return false;
    }
    
    infoLog << "init() minDelta=" << minDelta << " bufferLength=" << bufferLength << std::endl;
    this->minDelta = minDelta;
    this->bufferLength = bufferLength;
    this->numInputDimensions = numDimensions;
    this->numOutputDimensions = numInputDimensions;
    featureDataReady = false;

    initPeakDetection(numDimensions);

    //Resize the feature vector and fill it with zeros
    featureMatrix.resize(bufferLength, numInputDimensions, 0);

    //Resize the raw trajectory data buffer
    dataBuffer.resize( bufferLength, VectorFloat(numInputDimensions,0) );
    currentPosition = 0;

    featureVector.resize(numOutputDimensions,0);
    infoLog << "featureVector d=" << featureVector.size() << std::endl;

    //Flag that the zero crossing counter has been initialized
    initialized = true;
    
    return true;
}

VectorFloat LocalPeakDetection::update(const Float x){
    return update(VectorFloat(1,x));
}

void LocalPeakDetection::initPeakDetection(int numDimensions) {
    infoLog << "initPeakDetection d=" << numDimensions << std::endl;
    /*
    int lookformax = 1;
    Float min = 10000;
    Float max = -10000;
    Float minPos = -1;
    Float maxPos = -1;
    */

    lookForMaxPerDimension.resize(numInputDimensions, 1);
    minPerDimension.resize(numInputDimensions, 10000);
    maxPerDimension.resize(numInputDimensions, -10000);
    minPosPerDimension.resize(numInputDimensions, -1);
    maxPosPerDimension.resize(numInputDimensions, -1);
}

/*
 * value
 * dimension
 * index
 *
 * based on http://www.billauer.co.il/peakdet.html
 *
 * return [ NAN NAN x NAN NAN ] or [ NAN -x NAN NAN NAN ] where x is the index of local min or max.
 * x < 0 means it's a min, x > 0 means its a max.
 * NAN means this point is neither mix nor max
 * Note that if x = 0 you still receives -0 or 0
 */

const void LocalPeakDetection::peak(const float v, const int d, const int i) {
    Float now = v;
    Float min = minPerDimension[d];
    Float max = maxPerDimension[d];
    Float minPos = minPosPerDimension[d];
    Float maxPos = maxPosPerDimension[d];
    Float lookformax = lookForMaxPerDimension[d];

    if(now > max) {
        max = now;
        maxPos = i;
        if(d==5)
        infoLog << "max=" << std::setprecision(2) << max << " (" << maxPos << ")" << std::endl;
    }

    if(now < min) {
        min = now;
        minPos = i;
        if(d==5)
        infoLog << "min=" << std::setprecision(2) << min << " (" << minPos << ")" << std::endl;
    }

    if(lookformax) {
        if(now < max-minDelta) {
            if(d==5)
            infoLog << "max " << std::setprecision(2) << max << " found at " << maxPos << std::endl;
            featureVector[d] = maxPos;
            min = now;
            minPos = i;
            lookformax = 0;
        }
    } else {
        if(now > min+minDelta) {
            if(d==5)
            infoLog << "min " << std::setprecision(2) << min << " found at " << minPos << std::endl;
            featureVector[d] = -minPos;
            max = now;
            maxPos = i;
            lookformax = 1;
        }
    }

    minPerDimension[d] = min;
    maxPerDimension[d] = max;
    minPosPerDimension[d] = minPos;
    maxPosPerDimension[d] = maxPos;
    lookForMaxPerDimension[d] = lookformax;
}

/*
 *
 */

VectorFloat LocalPeakDetection::update(const VectorFloat &x){

    #ifdef GRT_SAFE_CHECKING
    if( !initialized ){
        errorLog << "update(const VectorFloat &x) - Not Initialized!" << std::endl;
        return VectorFloat();
    }

    if( x.getSize() != numInputDimensions ){
        errorLog << "update(const VectorFloat &x)- The Number Of Input Dimensions (" << numInputDimensions << ") does not match the size of the input vector (" << x.getSize() << ")!" << std::endl;
        return VectorFloat();
    }
    #endif

    //infoLog << "LocalPeakDetection::update from vector size=" << x.size() << std::endl;

    //Add the new data to the trajectory data buffer
    const unsigned int index = dataBuffer.getWritePointerPosition();
    //infoLog << "currentPosition=" << currentPosition << " index=" << index << std::endl;

    dataBuffer.push_back( x );
    featureVector.fill(NAN);

    // For each dimension
    for(UINT d=0; d<numInputDimensions; d++){
        peak(x[d], d, currentPosition); // will set feature vector
    }

    /*
    if(currentPosition == 0) {
        featureVector[0] = 0.0;
        featureVector[1] = -0.0;
        featureVector[2] = NAN;
        infoLog << featureVector[0] << " " << featureVector[1] << " " << featureVector[2] << std::endl;
    }
    */

    currentPosition++;

    //Only flag that the feature data is ready if the trajectory data is full
    if( !dataBuffer.getBufferFilled() ){
        featureDataReady = false;
    } else
        featureDataReady = true;

    return featureVector;
}

/*
 *
 */

const bool LocalPeakDetection::update(const MatrixFloat &x){

    if( !initialized ){
        errorLog << "update(const MatrixFloat &x) - Not initialized!" << std::endl;
        return false;
    }

    if( x.getNumCols() != numInputDimensions ){
        errorLog << "update(const MatrixFloat &x) - The number of columns in the inputMatrix (" << x.getNumCols() << ") does not match that of the FeatureExtraction (" << numInputDimensions << ")!" << std::endl;
        return false;
    }

    infoLog << "LocalPeakDetection::update from matrix rows=" << x.getNumRows() << " col=" << x.getNumCols() << std::endl;

    featureMatrix.resize(x.getNumRows(), numInputDimensions, 0);

    featureDataReady = false;

    dataBuffer.clear(); // set getNumValuesInBuffer to 0
    currentPosition = 0;

    // For each row (data sample)
    for(UINT k=0; k<x.getNumRows(); k++){
        VectorFloat f = update(x.getRow(k));
        featureMatrix.setRowVector(f, k);
    }

    featureDataReady = true;

    return true;
}

const Float LocalPeakDetection::getMinDelta() const {
    return minDelta;
}

const long LocalPeakDetection::getBuferLength() const {
    return bufferLength;
}

/*
 *
 */

CircularBuffer< VectorFloat > LocalPeakDetection::getData() const{
    if( initialized ){
        return dataBuffer;
    }
    return CircularBuffer< VectorFloat >();
}


GRT_END_NAMESPACE
