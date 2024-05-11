import cv2
import numpy as np
from tensorflow.keras.models import load_model
from tensorflow.keras.preprocessing import image
import serial
import time

# Define your class labels
class_labels = ["Metal", "Organic", "Paper", "Plastic"]

# Initialize serial communication with Arduino
ser = serial.Serial('COM7', 9600)  # Change 'COM7' to the port where your Arduino is connected
time.sleep(2)  # Allow time for Arduino to initialize

# Function to predict waste type from an image
def predict_waste_type(image_path):
    model = load_model('AWS_CNN.h5')
    print("In predict_waste_type()")

    # Load the image from the provided path
    img = image.load_img(image_path, target_size=(300, 300))
    img_array = image.img_to_array(img)
    img_array = np.expand_dims(img_array, axis=0)
    img_array /= 255.  # Normalize the image

    prediction = model.predict(img_array)
    predicted_class_index = np.argmax(prediction)
    predicted_class = class_labels[predicted_class_index]
    print("Predicted Class:", predicted_class)
    print("Confidence:", prediction[0][predicted_class_index])

    # Send the predicted waste class to Arduino
    if predicted_class in class_labels:
        ser.write(predicted_class.encode())  # Send the class trigger as string
        print("Sent class trigger to Arduino:", predicted_class)

    # Wait for acknowledgement from Arduino
    while True:
        if ser.in_waiting > 0:
            message = ser.readline().decode().strip()
            if message == "Done":
                print("Waste prediction and trapdoor operation complete.")
                break

# Main function to continuously listen for commands from Arduino
def main():
    print("Listening for commands...")
    while True:
        # Wait for command from Arduino
        command = ser.readline().decode().strip()
        print("Command received:", command)

        if command == "detect":
            print("IR Sensor detected something.")

            # Provide the path to the image you want to use for prediction
            image_path = "cabbage.png"
            predict_waste_type(image_path)  # Predict waste type using the provided image
            time.sleep(2)  # Wait for a while before detecting again

# Call the main function
if __name__ == "__main__":
    main()
