# capthca-reader
This is the final project for the Fundamentals of Programming course held in fall of 2018 written in C.

The project aim was to use to a pre written library to process .BMP images and modify a database of letters in order to use them for recognizing patterns in a Captcha image.

The project is consisted of several parts the first one includes reading the dataset and cropping the images on their margins to create an image of a single letter without any white spaces around it.

The second part is resizing the cropped images so that they all fit a similar resolution and mainting the pixels color integrity during the process.

The third part is creating a single image for each letter from the dataset by averaging all the images for each letter and turning them into a new one containing all of them.

The final part of the project is scanning the image filled with different captchas and then recognizing each letter in each column and then searching for the user's input pattern within the image. If it was found, then the program would alter the image by colorizing the found pattern
