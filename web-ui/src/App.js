import React, { useEffect, useRef } from 'react';
import './App.css';
import { BrowserRouter, Route, Routes } from 'react-router-dom';
import ControlsPage from './pages/ControlsPage';
import DevPage from "./pages/DevPage";

function App() {
  // source: https://ably.com/blog/websockets-react-tutorial
  const connection = useRef(null);

  useEffect(() => {
    const socket = new WebSocket("ws://192.168.4.1/ws") // change to server IP

    // Connection opened
    socket.addEventListener("open", (event) => {
      socket.send("Connection established")
    })

    // Listen for messages
    socket.addEventListener("message", (event) => {
      console.log("Message from server ", event.data)
    //   alert("Message from server ", event.data)
    })

    connection.current = socket

    return () => connection.close()
  }, []);

  return (
    <BrowserRouter>
      <Routes>
        <Route path="/" element={<ControlsPage />}></Route>
        <Route path="/dev" element={<DevPage />}></Route>
      </Routes>
    </BrowserRouter>
    
  );
}

export default App;
