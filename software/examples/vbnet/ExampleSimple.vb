Imports System
Imports Tinkerforge

Module ExampleSimple
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Particulate Matter Bricklet

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim pm As New BrickletParticulateMatter(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get current PM concentration
        Dim pm10, pm25, pm100 As Integer

        pm.GetPMConcentration(pm10, pm25, pm100)

        Console.WriteLine("PM 1.0: " + pm10.ToString() + " µg/m³")
        Console.WriteLine("PM 2.5: " + pm25.ToString() + " µg/m³")
        Console.WriteLine("PM 10.0: " + pm100.ToString() + " µg/m³")

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
