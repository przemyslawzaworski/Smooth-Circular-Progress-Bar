using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Diagnostics;
using System;
using System.Runtime.InteropServices;
using System.Text;
using System.IO;

public class Demo : MonoBehaviour
{
	public Vector2Int Offset = new Vector2Int(100, 100);

	[DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
	static extern int GetWindowText(IntPtr hWnd, StringBuilder lpString, int nMaxCount);

	IntPtr _IntPtr;
	Process _Process;
	bool _Simulation = false;

	void StartProgressBar()
	{
		if (_Process != null) return;
		_IntPtr = Process.GetCurrentProcess().MainWindowHandle;
		StringBuilder stringBuilder = new StringBuilder(255);
		GetWindowText(_IntPtr, stringBuilder, 256);
		string title = stringBuilder.ToString();
		_Process = new Process();
		_Process.StartInfo = new ProcessStartInfo();
		_Process.StartInfo.FileName = Path.Combine(Application.streamingAssetsPath, "Demo.exe");
		_Process.StartInfo.Arguments = Offset.x.ToString() + " " + Offset.y.ToString() + " " + title; 
		_Process.Start();
	}

	void StopProgressBar()
	{
		if (_Process != null)
		{
			_Process.Kill();
			_Process = null;
		}
	}

	void StartSimulation()
	{
		for (uint i = 0; i < 2e8; i++)
		{
			float n = Mathf.Sin(i);
		}
	}

	void Update()
	{
		if (Input.GetKeyDown(KeyCode.Space))
		{
			StartProgressBar();
			StartSimulation();
			StopProgressBar();
		}
	}

	void OnGUI()
	{
		float x = Screen.width * (Mathf.Sin(Time.time) * 0.5f + 0.5f);
		GUI.DrawTexture(new Rect(0, 0, x, Screen.height), Texture2D.blackTexture, ScaleMode.ScaleToFit, false, x / (float)Screen.height);
	}
}