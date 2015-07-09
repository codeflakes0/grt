/**
 @author  Nicholas Gillian <nick@nickgillian.com>
 @brief This file implements a basic tool for processing data files and training a random forest model.
 @date 7-July-2015
*/

#include <GRT/GRT.h>
using namespace GRT;

InfoLog infoLog("[grt-rf-tool]");
WarningLog warningLog("[WARNING grt-rf-tool]");
ErrorLog errorLog("[ERROR grt-rf-tool]");

bool printUsage(){
    infoLog << "grt-rf-tool [options]\n";
    infoLog << "\t-m: sets the current application mode, can be: \n";
    infoLog << "\t\t[1] 'train-model': trains a random forest model, using a pre-recorded training dataset.\n";
    infoLog << "\t\t[2] 'combine-models': combines multiple random forest models into a single random forest model.\n";
    infoLog << "\t-f: sets the main filename. \n";
    infoLog << "\t\tIn 'train-model' mode, this sets the name of the file the training data will be loaded from.\n";
    infoLog << "\t\tIn 'combine-models' mode this sets the name of the file the combined model will be saved to.\n";
    infoLog << "\t-d: sets the data directory containing the raw csv files that will be combined into the main dataset, only needed for 'combine-files' mode\n";
    infoLog << "\t\tIn 'combine-models' mode, this sets the data directory containing the random forest model files that will be combined into the main random forest model.\n";
    infoLog << "\t--model: sets the filename the random forests model will be saved to\n";
    infoLog << "\t--forest-size: sets the number of tress in the forest, only used for 'train-model'\n";
    infoLog << "\t--max-depth: sets the maximum depth the forest can reach, only used for 'train-model'\n";
    infoLog << "\t--min-node-size: sets the minimum number of training samples allowed per node, only used for 'train-model'\n";
    infoLog << "\t--num-splits: sets the number of random splits allowed per node, only used for 'train-model'\n";
    infoLog << endl;
    return true;
}

bool train( CommandLineParser &parser );
bool combineModels( CommandLineParser &parser );

int main(int argc, char * argv[])
{

    if( argc < 2 ){
        errorLog << "Not enough input arguments!" << endl;
        printUsage();
        return EXIT_FAILURE;
    }

    //Create an instance of the parser
    CommandLineParser parser;

    //Disable warning messages
    parser.setWarningLoggingEnabled( false );

    //Add some options and identifiers that can be used to get the results
    parser.addOption( "-m", "mode" );
    parser.addOption( "-f", "filename" );
    parser.addOption( "-d", "data-dir" );
    parser.addOption( "--model", "model-filename" );
    parser.addOption( "--results", "results-filename" );
    parser.addOption( "--forest-size", "forest-size" );
    parser.addOption( "--max-depth", "max-depth" );
    parser.addOption( "--min-node-size", "min-node-size" );
    parser.addOption( "--num-splits", "num-splits" );

    //Parse the command line
    parser.parse( argc, argv );

    string mode = "";
    string filename = "";

    //Get the application mode
    if( !parser.get("mode",mode) ){
        errorLog << "Failed to parse mode from command line! You can set the mode using the -m option." << endl;
        printUsage();
        return EXIT_FAILURE;
    }

    //Train RF model
    if( mode == "train-model" ){
        if( train( parser ) ){
            infoLog << "Model Trained!" << endl;
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    //Combine models mode
    if( mode == "combine-models" ){
        if( combineModels( parser ) ){
            infoLog << "Models combined." << endl;
            return EXIT_SUCCESS;
        }else{
            errorLog << "Failed to combine models!" << endl;
            return EXIT_FAILURE;
        }
    }

    errorLog << "Unknown application mode: " << mode << endl;
    printUsage();

    return EXIT_FAILURE;
}

bool train( CommandLineParser &parser ){

    infoLog << "Training Random Forest model..." << endl;

    string trainDatasetFilename = "";
    string modelFilename = "";
    string defaultFilename = "rf-model.grt";
    unsigned int numThreads = 0;
    unsigned int forestSize = 0;
    unsigned int maxDepth = 0;
    unsigned int minNodeSize = 0;
    unsigned int numSplits = 0;
    unsigned int defaultForestSize = 5;
    unsigned int defaultMaxDepth = 10;
    unsigned int defaultMinNodeSize = 50;
    unsigned int defaultNumSplits = 100;

    //Get the filename
    if( !parser.get("filename",trainDatasetFilename) ){
        errorLog << "Failed to parse filename from command line! You can set the filename using the -f." << endl;
        printUsage();
        return false;
    }

    //Get the model filename
    parser.get("model-filename",modelFilename,defaultFilename);

    //Get the forest size
    parser.get("forest-size",forestSize,defaultForestSize);

    //Get the max depth
    parser.get("max-depth",maxDepth,defaultMaxDepth);

    //Get the min node size
    parser.get("min-node-size",minNodeSize,defaultMinNodeSize);

    //Get the number of random splits
    parser.get("num-splits",numSplits,defaultNumSplits);

    //Load some training data to train the classifier
    ClassificationData trainingData;

    infoLog << "- Loading Training Data..." << endl;
    if( !trainingData.load( trainDatasetFilename ) ){
        errorLog << "Failed to load training data!\n";
        return false;
    }

    const unsigned int N = trainingData.getNumDimensions();
    infoLog << "- Num training samples: " << trainingData.getNumSamples() << endl;
    infoLog << "- Num dimensions: " << N << endl;
    infoLog << "- Num classes: " << trainingData.getNumClasses() << endl;

    //Create a new RandomForests instance
    RandomForests forest;

    //Set the decision tree node that will be used for each tree in the forest
    forest.setDecisionTreeNode( DecisionTreeClusterNode() );

    //Set the number of trees in the forest
    forest.setForestSize( forestSize );

    //Set the maximum depth of the tree
    forest.setMaxDepth( maxDepth );

    //Set the minimum number of samples allowed per node
    forest.setMinNumSamplesPerNode( minNodeSize );

    forest.setNumRandomSplits( numSplits );

    //Add the classifier to a pipeline
    GestureRecognitionPipeline pipeline;
    pipeline.setClassifier( forest );

    infoLog << "- Training model...\n";

    //Train the classifier
    if( !pipeline.train( trainingData ) ){
        errorLog << "Failed to train classifier!" << endl;
        return false;
    }

    infoLog << "- Model trained!" << endl;

    infoLog << "- Saving model to: " << modelFilename << endl;

    //Save the pipeline
    if( pipeline.save( modelFilename ) ){
        infoLog << "- Model saved." << endl;
    }else warningLog << "Failed to save model to file: " << modelFilename << endl;

    infoLog << "- TrainingTime: " << pipeline.getTrainingTime() << endl;
    infoLog << "- NumThreads: " << ThreadPool::getThreadPoolSize() << endl;

    return true;
}

bool combineModels( CommandLineParser &parser ){

    infoLog << "Combining models..." << endl;

    string directoryPath = "";
    string modelFilename = "";

    if( !parser.get("data-dir",directoryPath) ){
        errorLog << "Failed to parse data-directory from command line! You can set the data-directory using the --data-dir option." << endl;
        printUsage();
        return false;
    }

    //Get the filename
    if( !parser.get("model-filename",modelFilename) ){
        errorLog << "Failed to parse filename from command line! You can set the model filename using the --model." << endl;
        printUsage();
        return false;
    }

    vector< string > files;

    infoLog << "- Parsing data directory: " << directoryPath << endl;

    //Parse the directory to get all the csv files
    if( !Util::parseDirectory( directoryPath, ".grt", files ) ){
        errorLog << "Failed to parse data directory!" << endl;
        return false;
    }

    typedef std::shared_ptr<GestureRecognitionPipeline> GestureRecognitionPipelinePtr;
    auto mainPipeline = std::make_shared<GestureRecognitionPipeline>();
    RandomForests forest;
    std::vector< GestureRecognitionPipelinePtr > pipelineBuffer;
    unsigned int inputVectorSize = 0;
    const unsigned int numFiles = (unsigned int)files.size();
    bool mainPipelineSet = false;
    
    //Loop over the files, load them, and add valid random forest pipelines to the pipelineBuffer so they can be combined with the mainPipeline
    for(unsigned int i=0; i<numFiles; i++){
        infoLog << "- Loading model " << files[i] << ". File " << i+1 << " of " << numFiles << endl;

        auto pipeline = std::make_shared< GestureRecognitionPipeline >();

        if( pipeline->load( files[i] ) ){

            infoLog << "- Pipeline loaded. Number of input dimensions: " << pipeline->getInputVectorDimensionsSize() << endl;

            if( pipelineBuffer.size() == 0 ){
                inputVectorSize = pipeline->getInputVectorDimensionsSize();
            }

            if( pipeline->getInputVectorDimensionsSize() != inputVectorSize ){
                warningLog << "- Pipeline " << i+1 << " input vector size does not match the size of the first pipeline!" << endl;
            }else{

                Classifier *classifier = pipeline->getClassifier();
                if( classifier ){
                    if( classifier->getClassifierType() == forest.getClassifierType() ){ //Validate the classifier is a random forest
                        if( !mainPipelineSet ){
                            mainPipelineSet = true;
                            mainPipeline = pipeline;
                        }else pipelineBuffer.push_back( pipeline );
                    }else{
                        warningLog << "- Pipeline " << i+1 << " does not contain a random forest classifer! Classifier type: " << classifier->getClassifierType() << endl;
                    }
                }

            }
        }else{
            warningLog << "- WARNING: Failed to load model from file: " << files[i] << endl;
        }
    }

    if( mainPipelineSet ){

        //Combine the random forest models with the main pipeline model
        const unsigned int numPipelines = (unsigned int)pipelineBuffer.size();
        RandomForests *mainForest = mainPipeline->getClassifier< RandomForests >();

        for(unsigned int i=0; i<numPipelines; i++){

            infoLog << "- Combing model " << i+1 << " of " << numPipelines << " with main model..." << endl;

            RandomForests *f = pipelineBuffer[i]->getClassifier< RandomForests >();

            if( !mainForest->combineModels( *f ) ){
                warningLog << "- WARNING: Failed to combine model " << i+1 << " with the main model!" << endl;
            }

        }

        if( mainPipeline->getTrained() ){
            infoLog << "- Saving combined pipeline to file..." << endl;
            return mainPipeline->save( modelFilename );
        }

    }else{
        errorLog << "Failed to combined models, no models were loaded!" << endl;
        return false;
    }

    return true;
}
