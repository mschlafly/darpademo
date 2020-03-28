using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

using Windows.Storage;
using Windows.UI.ViewManagement;
using Windows.Storage.Pickers;

namespace sharedcontrol
{
    public sealed partial class MainPage : Page
    {
        // Size of haptic display
        int W_tablet = 1280; //738;
        int H_tablet = 780; //1024;

        // Distance in the unity environment to show on tablet (for height)
        int H_unity = 30; // A value of 30 mean that the entire square environement is displayed on the tablet

        // Position of person in tablet
        int x_person_tablet;
        int y_person_tablet;
        // Ratio for zooming
        float zoom_ratio;

        // Variables for saving data to txt file
        StorageFile newFile;
        bool file_created = false;
        bool collectdata = false; // Boolean for whether to save the data 

        // For saving and recieving info from files
        String string_of_coordinates = "";
        MediaElement mysong_start = new MediaElement();
        MediaElement mysong_end = new MediaElement();

        public MainPage()
        {
            this.InitializeComponent();
            ApplicationView.GetForCurrentView().TryEnterFullScreenMode();

            // Position of person in tablet
            x_person_tablet = W_tablet / 2;
            y_person_tablet = H_tablet / 2;
            // Ratio for zooming
            zoom_ratio = H_tablet / H_unity;

            mainCanvas.DoubleTapped += new DoubleTappedEventHandler(target_DoubleTapped);
            mainCanvas.PointerMoved += new PointerEventHandler(Pointer_Moved);
            pickfiles();
        }

        // This functin is run at the beginning and opens the picker to select files from the 
        // downloads folder to make sounds and to save the touch coordinates to
        private async void pickfiles()
        {
            System.Diagnostics.Debug.WriteLine("Pick audio file sound1.wav from Downloads folder");
            FileOpenPicker openPicker = new FileOpenPicker();
            openPicker.ViewMode = PickerViewMode.Thumbnail;
            openPicker.SuggestedStartLocation = PickerLocationId.Downloads;
            openPicker.FileTypeFilter.Add(".wav");
            StorageFile file = await openPicker.PickSingleFileAsync();
            var stream = await file.OpenAsync(Windows.Storage.FileAccessMode.Read);
            mysong_start.SetSource(stream, file.ContentType);

            System.Diagnostics.Debug.WriteLine("Pick audio file sound2.wav from Downloads folder");
            FileOpenPicker openPicker3 = new FileOpenPicker();
            openPicker3.ViewMode = PickerViewMode.Thumbnail;
            openPicker3.SuggestedStartLocation = PickerLocationId.Downloads;
            openPicker3.FileTypeFilter.Add(".wav");
            file = await openPicker.PickSingleFileAsync();
            stream = await file.OpenAsync(Windows.Storage.FileAccessMode.Read);
            mysong_end.SetSource(stream, file.ContentType);

            System.Diagnostics.Debug.WriteLine("Pick text file touch_locations.txt to write to.");
            FileOpenPicker openPicker2 = new FileOpenPicker();
            openPicker2.ViewMode = PickerViewMode.Thumbnail;
            openPicker2.SuggestedStartLocation = PickerLocationId.Downloads;
            openPicker2.FileTypeFilter.Add(".txt");
            newFile = await openPicker2.PickSingleFileAsync();

            // Saves empty string of coordinates to file 
            savefile();
        }
        // If the a double-tap is detected, either begin collecting data or save the data, 
        // depending on the state of the collectdata variable
        void target_DoubleTapped(object sender, DoubleTappedRoutedEventArgs e)
        {

            if (collectdata == true)
            {
                System.Diagnostics.Debug.WriteLine("Double Tap Detected - Saving data.");

                mysong_end.Play();  // Plays audio file
                savefile();
                collectdata = false;
                string_of_coordinates = "";

            }
            else
            {
                System.Diagnostics.Debug.WriteLine("Double Tap Detected - Data collection starting now.");
                mysong_start.Play();  // Plays audio file
                collectdata = true;
            }
        }

        // If the pointer is moved and you are collecting data, 
        // convert the pixel coordinates to "Unity" coordinates (a 30x30 grid with the origin in the lower left hand corner),
        // and save the data to a string
        void Pointer_Moved(object sender, PointerRoutedEventArgs e)
        {

            if (collectdata == true)
            {
                // Retrieve the point associated with the current event
                Windows.UI.Input.PointerPoint currentPoint = e.GetCurrentPoint(mainCanvas);
                // translate the coordinates so that the person is at (0,0)
                double tempx = currentPoint.Position.X - x_person_tablet;
                double tempy = currentPoint.Position.Y - y_person_tablet;
                // scale to unity coordinate system
                tempx = tempx / zoom_ratio;
                tempy = tempy / zoom_ratio;
                // NOTE THE REST OF THE CONVERSION HAS BEEN REMOVES AND PERFORMED AFTER SENDING OVER TCP SOCKET
                int x_unity = Convert.ToInt32(Math.Round(tempx));
                int y_unity = Convert.ToInt32(Math.Round(tempy));

                string_of_coordinates = string_of_coordinates + x_unity.ToString() + ',' + y_unity.ToString() + ',';

                System.Diagnostics.Debug.WriteLine("Position: X- {0} Y- {1}", x_unity, y_unity);
            }
        }

        // Save the string_of_coordinates to touch_locations.txt
        private async void savefile()
        {
            bool need_save_file = true;
            String string_of_coordinates2 = string_of_coordinates;
            while (need_save_file)
            {
                need_save_file = false;
                try
                {
                    await Windows.Storage.FileIO.WriteTextAsync(newFile, string_of_coordinates2);
                    System.Diagnostics.Debug.WriteLine("Tried to save file");
                }
                catch
                {
                    System.Diagnostics.Debug.WriteLine("Exception occurred while saving file.");
                    need_save_file = true;
                }

            }

        }
    }
}
