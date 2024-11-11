# The purpose of this document is to explain the files inside the repository

## Microcontroller project

### Structure
The project that is to be opened in the STM32IDE is the GENX320_STM32_V2.0.0 which is in theDemo_project_by_Prophesee
directory together with the Demo Compilation Instruction.pdf.
The main directory used in this project where all the necessary scripts are and where the main action happens is
GENX320_STM32_V2.0.0/STM32CubeIDE/Application/User/Core.

### Predictor script
The script predictor.h contains all the name and parameters
of methods related to the CNN, while predictor.c contains their declaration. In predictor.c script
predict_main(float* img) is the main method that runs the image through all the layers and returns the predicted digit.
The method predict_main is used in task_frame_buffer.c.

### Task frame buffer
task_frame_buffer.c is the script that does all the action. It contains the method task_update_fb that runs an infinite
loop. In this loop happens the analysis of each frame. The frame is stored every loop in the frame_buffer_l1_one
variable. At the end of the loop(read the comments in the code) happens the preprocessing of a 252x252 frame region from
the frame_buffer_l1_one. The 252x252 region(red square region on the microcontroller screen) is compressed 9 times to a
28x28 image stored in buffer_number that would match the input necessary for the CNN. Inside the variable "prediction" is
stored the output from the predict_main function mentioned above. The loop also contains some variables that can help
assess the performance like start_time, end_time, time_taken. The prediction variable is used in the get_prediction()
to just return the prediction.

### GUI
In order to show the predicted digit on the screen a connection needs to be made between the prediction variable and
the GUI. This is done by using the get_prediction() method mentioned above inside TouchGFX/gui/src/screen_1_screen/Screen_1View.cpp.
The method updateEvents() contains the label from the screen and it takes the value from get_prediction(). The project
contains multiple screens and the microcontroller is touchscreen. All teh interfaces can be change in the MXDesigner
software.

## Python Scripts

### Image Creator
ImageCreator.py takes the imgs.txt file that contains 200 images from the MNIST test set in the format of arrays and
converts them to actual .png 28x28 images that can be shown to the camera. This script was used for testing purposes.
We created a video of the 200 images and used it to show the demo.

### Quantized weights
The file quantized_weights.txt contains the weights quantized to int16_t.

### Weights format C
The script weights_format_for_c.py was used to transform the weights from quantized_weights.txt to a format that can be
copied straight into a C script as a list so you don't have to read them on the microcontroller and save them there.
The weights in the mentioned format can be found in GENX320_STM32_V2.0.0/STM32CubeIDE/Application/User/Core/weights.c.


