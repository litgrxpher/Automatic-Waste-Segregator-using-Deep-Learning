import cv2
import numpy as np
from tensorflow.keras.models import load_model
from tensorflow.keras.preprocessing import image
import keyboard as kb

# Define your class labels
class_labels = ["Metal","Organic","Paper","Plastic"]  

cap = cv2.VideoCapture(0)
cap.set(3, 300)
cap.set(4, 300)

def predict():
    model = load_model('AWS_CNN.h5')

    success, img2 = cap.read()
    cv2.imwrite("image.jpeg", img2)

    img = image.load_img("image.jpeg", target_size=(300, 300))
    img_array = image.img_to_array(img)
    img_array = np.expand_dims(img_array, axis=0)
    img_array /= 255.  # Normalize the image

    prediction = model.predict(img_array)
    predicted_class_index = np.argmax(prediction)
    predicted_class = class_labels[predicted_class_index]
    print("Predicted Class:", predicted_class)
    print("Confidence:", prediction[0][predicted_class_index])
    global flag
    flag=0
flag=0
running = True
while running:
    if flag==0:
        print("Press Q to Capture")
        flag=1
    if kb.is_pressed('q'):
        predict()

img = cv2.imread("image.jpeg")
while True:
    cv2.imshow("output", img)
    cv2.waitKey(1)
