Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Particulate Matter Bricklet

    ' Callback subroutine for PM concentration callback
    Sub PMConcentrationCB(ByVal sender As BrickletParticulateMatter, _
                          ByVal pm10 As Integer, ByVal pm25 As Integer, _
                          ByVal pm100 As Integer)
        Console.WriteLine("PM 1.0: " + pm10.ToString() + " µg/m³")
        Console.WriteLine("PM 2.5: " + pm25.ToString() + " µg/m³")
        Console.WriteLine("PM 10.0: " + pm100.ToString() + " µg/m³")
        Console.WriteLine("")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim pm As New BrickletParticulateMatter(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register PM concentration callback to subroutine PMConcentrationCB
        AddHandler pm.PMConcentrationCallback, AddressOf PMConcentrationCB

        ' Set period for PM concentration callback to 1s (1000ms)
        pm.SetPMConcentrationCallbackConfiguration(1000, False)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
