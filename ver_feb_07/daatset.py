import os
import torch
import torchaudio
import numpy as np
import sounddevice as sd
import wave
from scipy.spatial.distance import cosine
from scipy.signal import butter, filtfilt
from transformers import Wav2Vec2Processor, Wav2Vec2Model
from sklearn.svm import SVC
from sklearn.neighbors import KNeighborsClassifier
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import accuracy_score
import librosa
import pickle
# Main execution
def main():
    # Check if a trained model exists
    model_filename = "voice_model.pkl"
    if os.path.exists(model_filename):
        try:
            print(f"Loading existing model from {model_filename}...")
            clf, scaler, word_templates = load_model(model_filename)
            print("Model loaded successfully!")
        except Exception as e:
            print(f"Error loading model: {e}")
            clf, scaler, word_templates = None, None, None
    else:
        clf, scaler, word_templates = None, None, None
    
    # If no model exists or loading failed, train a new one
    if clf is None:
        # Step 1: Prepare training data
        print("Preparing training data...")
        X, y = prepare_training_data(dataset_path)
        
        if len(X) > 0:
            # Step 2: Train classifier
            print("Training classifier...")
            clf, scaler = train_classifier(X, y, classifier_type='svm')
            
            # Prepare DTW templates
            word_templates = {}
            for word in set(y):
                word_indices = np.where(y == word)[0]
                word_templates[word] = np.mean([X[i] for i in word_indices], axis=0)
            
            # Save the model
            save_model(clf, scaler, word_templates, model_filename)
        else:
            print("No training data available. Cannot train a model.")
            return
    
    # Step 3: Record and recognize
    print("\nVoice recognition system ready!")
    print("Available commands: " + ", ".join(word_templates.keys()))
    
    while True:
        try:
            user_input = input("\nPress Enter to start recording (or type 'q' to quit): ")
            if user_input.lower() == 'q':
                break
                
            audio_data =  user_input
            
            # Method 1: ML Classifier
            ml_prediction = predict_word(audio_data, clf, scaler)
            print(f"ML Prediction: {ml_prediction}")
            
            # Method 2: DTW
            dtw_prediction, dtw_distance = dtw_recognition(audio_data, word_templates)
            print(f"DTW Prediction: {dtw_prediction} (Distance: {dtw_distance:.4f})")
            
            # Combine predictions (simple ensemble)
            if ml_prediction == dtw_prediction:
                final_prediction = ml_prediction
                confidence = "High"
            else:
                # Use ML prediction as default
                final_prediction = ml_prediction  
                confidence = "Medium"
            
            print(f"\nFinal Recognition Result: {final_prediction} (Confidence: {confidence})")
        
        except KeyboardInterrupt:
            print("\nExiting...")
            break
        except Exception as e:
            print(f"Error during recognition: {e}")

if __name__ == "__main__":
    main()