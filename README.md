# GreekLetterRecognition
 Implementation of neural network and gui for generating training data and testing functionality
 
 Program is designed for recognition of letters "alpha","beta","gamma","delta","etha".
 Through gui you can generate train data for neural network. 
    python gui_denerate_data.py {number_of_iterations}
 Program takes one argument ( integer ). You will have to draw *number_of_iterations* cycles of letters.
 So if *number_of_iterations* is 10 -> you will have to draw alpha,beta,gamma,delta and etha 10 times and in a cycle, total of 50 letters.
 
Generate data for testing by running : 
  python gui_generate_test_data.py {arg}
This time you will have to draw  *arg* letters.

To train and test the neural network - compile the code then run it with 2 arguments
 Argument 1 : number of key points to be extracted from the drawings
 Arguement 2: which form of gradient descent to use - 1 for stohastic, 2 for mini-batch , 3 for batch
ex:
./a 7 3

The output of predictions for testing data will be printed in the console.
  
  
