/*
 * CodeFlakes
 *
 * Ported from http://www.billauer.co.il/peakdet.html
 */

#ifndef GRT_LOCAL_PEAK_DETECTION_HEADER
#define GRT_LOCAL_PEAK_DETECTION_HEADER

#include "../../CoreModules/FeatureExtraction.h"
#include "../../PreProcessingModules/Derivative.h"
#include "../../PreProcessingModules/DeadZone.h"

GRT_BEGIN_NAMESPACE

class GRT_API LocalPeakDetection : public FeatureExtraction{
public:

    LocalPeakDetection(const UINT numDimensions=1, const Float minDelta=0.5,const ULONG bufferLength=100);
    
    LocalPeakDetection(const LocalPeakDetection &rhs);
    virtual ~LocalPeakDetection();
    LocalPeakDetection& operator=(const LocalPeakDetection &rhs);
    
    /**
    Sets the FeatureExtraction deepCopyFrom function, overwriting the base FeatureExtraction function.
    This function is used to deep copy the values from the input pointer to this instance of the FeatureExtraction module.
    This function is called by the GestureRecognitionPipeline when the user adds a new FeatureExtraction module to the pipeleine.
    
    @param featureExtraction: a pointer to another instance of a LocalPeakDetection, the values of that instance will be cloned to this instance
    @return returns true if the deep copy was successful, false otherwise
    */
    virtual bool deepCopyFrom(const FeatureExtraction *featureExtraction);
    
    /**
    Sets the FeatureExtraction computeFeatures function, overwriting the base FeatureExtraction function.
    This function is called by the GestureRecognitionPipeline when any new input data needs to be processed (during the prediction phase for example).
    This function calls the LocalPeakDetection's update function.
    
    @param inputVector: the inputVector that should be processed.  Must have the same dimensionality as the FeatureExtraction module
    @return returns true if the data was processed, false otherwise
    */
    virtual bool computeFeatures(const VectorFloat &inputVector);
    
    /**
    Sets the FeatureExtraction reset function, overwriting the base FeatureExtraction function.
    This function is called by the GestureRecognitionPipeline when the pipelines main reset() function is called.
    This function resets the feature extraction by re-initiliazing the instance.
    
    @return true if the filter was reset, false otherwise
    */
    virtual bool reset();
    
    /**
    This saves the feature extraction settings to a file.
    This overrides the save function in the FeatureExtraction base class.
    
    @param file: a reference to the file to save the settings to
    @return returns true if the settings were saved successfully, false otherwise
    */
    virtual bool save( std::fstream &file ) const;
    
    /**
    This loads the feature extraction settings from a file.
    This overrides the load function in the FeatureExtraction base class.
    
    @param file: a reference to the file to load the settings from
    @return returns true if the settings were loaded successfully, false otherwise
    */
    virtual bool load( std::fstream &file );
    
    bool init(const UINT numDimensions, const Float minDelta, const ULONG bufferLength);
    
    /**
    Computes the LocalPeakDetection features from the input, this should only be called if the dimensionality of this instance was set to 1.
    
    @param x: the value to compute features from, this should only be called if the dimensionality of the filter was set to 1
    @return a vector containing the LocalPeakDetection features, an empty vector will be returned if the features were not computed
    */
    VectorFloat update(const Float x);
    
    /**
    Computes the LocalPeakDetection features from the input, the dimensionality of x should match that of this instance.
    
    @param x: a vector containing the values to be processed, must be the same size as the numInputDimensions
    @return a vector containing the LocalPeakDetection features, an empty vector will be returned if the features were not computed
    */
    VectorFloat update(const VectorFloat &x);

    CircularBuffer< VectorFloat > getData() const;
    const Float getMinDelta() const;
    const long getBuferLength() const;
    const bool update(const MatrixFloat &x);

    //Tell the compiler we are using the following functions from the MLBase class to stop hidden virtual function warnings
    using MLBase::save;
    using MLBase::load;
    using MLBase::train;
    using MLBase::train_;
    using MLBase::predict;
    using MLBase::predict_;

    /**
    Gets a string that represents the LocalPeakDetection class.
    
    @return returns a string containing the ID of this class
    */
    static std::string getId();
    
private:
    const Float peak(const int v, const int d, const int i);
    void initPeakDetection(int numDimensions);

protected:
    Float minDelta;
    ULONG bufferLength;
    ULONG currentPosition;

    VectorFloat minPerDimension;
    VectorFloat maxPerDimension;
    VectorFloat minPosPerDimension;
    VectorFloat maxPosPerDimension;
    VectorFloat lookForMaxPerDimension;

    CircularBuffer< VectorFloat > dataBuffer;              ///< A buffer used to store a limited lenght of previous data

private:
    static RegisterFeatureExtractionModule< LocalPeakDetection > registerModule;
    static std::string id;
};

GRT_END_NAMESPACE

#endif //GRT_LOCAL_PEAK_DETECTION_HEADER
    
