import React from 'react';
import './App.css';
import { BrowserRouter, Route, Routes } from 'react-router-dom';
import ControlsPage from './pages/ControlsPage';
import DevPage from "./pages/DevPage";

function App() {

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
