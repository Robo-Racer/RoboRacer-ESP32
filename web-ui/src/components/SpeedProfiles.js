import React, { useState } from 'react';
import Box from '@mui/material/Box';
import TextField from '@mui/material/TextField';
import InputAdornment from '@mui/material/InputAdornment';
import FormControl from '@mui/material/FormControl';
import Select from '@mui/material/Select';
import InputLabel from '@mui/material/InputLabel';
import MenuItem from '@mui/material/MenuItem';
import { useMemo } from 'react';
import Button from '@mui/material/Button';
import speedProfilesData from '../data/speed_profiles.json';
import { DesktopTimePicker, LocalizationProvider } from '@mui/x-date-pickers';
import { AdapterDayjs } from '@mui/x-date-pickers/AdapterDayjs';

const dayjs = require('dayjs');

// grab the saved profiles from the json to populate dropdown
function useSpeedProfiles() {
    return useMemo(() => {
        return Object.keys(speedProfilesData).map((profileName) => ({
            value: profileName,
            label: profileName,
        }));
    }, []);
}

function SpeedProfiles() {
    const speedProfiles = useSpeedProfiles();
    // const [speedProfiles, setSpeedProfiles] = useState(getSpeedProfiles()); // json formatted profiles from file
    // const [formattedProfiles, setFormattedProfiles] = useState(formatSpeedProfiles()); // mapped profiles for select dropdown
    const [time, setTime] = useState(null);
    const [distance, setDistance] = useState('');
    const [selectedProfile, setSelectedProfile] = useState("");
    const [name, setName] = useState("");

    // async function getSpeedProfiles() {
    //     let response = await fetch('/speedProfiles')

    //     console.log("response:", response);
    // }

    // grab the saved profiles from the json to populate dropdown
    // function formatSpeedProfiles() {
    //     return Object.keys(speedProfiles).map((profileName) => ({
    //         value: profileName,
    //         label: profileName,
    //     }));
    // }

    function handleSelect(event) {
        const select = event.target.value;
        setSelectedProfile(select);

        if (select !== 'custom') {
            const minutes = speedProfiles[select].time / 60;
            const seconds = speedProfiles[select].time % 60;
            setTime(dayjs().minute(0).second(0));
            setDistance(0);
        }
        else {
            setTime(dayjs().minute(0).second(0));
            setDistance(0);
        }
    }

    // submit inputs and send to robot
    function handleClick() {
        // format time to be in seconds, distance to be an integer
        let seconds = time.second() + (time.minute() * 60);
        let meters = parseInt(distance);

        const data = {
            time: seconds,
            distance: meters
        }

        console.log('submit inputs:', data);

        fetch('/postData', { 
            method: 'POST',
            body: JSON.stringify(data)
        })
    }

    // send updated speed profile to robot
    function handleSave() {
        // format time to be in seconds, distance to be an integer
        // let seconds = time.second() + (time.minute() * 60);
        // let meters = parseInt(distance);

        // // append new speed profile and send entire JSON
        // const data = {
        //     ...speedProfilesData,
        //     [name]: {
        //         time: seconds,
        //         distance: meters
        //     }
        // }

        // console.log("new speed profiles", data)

        // fetch('/addSpeedProfile', {
        //     method: 'POST',
        //     body: JSON.stringify(data)
        // })

        // setSpeedProfiles(getSpeedProfiles()); // grab updated file
        // setFormattedProfiles(formatSpeedProfiles()); // reformat for dropdown
    }

    return (
        <Box
            component="form"
            autoComplete="off"
            className='box'
        >
            <FormControl fullWidth >
                <InputLabel id="simple-select-helper-label" sx={{ m: 1 }}>Speed Profile</InputLabel>
                <Select
                    id="speed-profiles"
                    label="Speed Profile"
                    sx={{ m: 1 }}
                    value={selectedProfile}
                    onChange={handleSelect}>
                    <MenuItem value={"custom"}>Custom</MenuItem>
                    {speedProfiles.map((profile) => (
                        <MenuItem key={profile.value} value={profile.value}>
                            {profile.label}
                        </MenuItem>
                    ))}
                </Select>

                <LocalizationProvider dateAdapter={AdapterDayjs}>
                    <DesktopTimePicker
                        label="Time"
                        id="time-input"
                        sx={{ m: 1 }}
                        value={time}
                        views={['minutes', 'seconds']}
                        format="mm:ss"
                        onChange={(newTime) => setTime(newTime)}
                        onError={() => {}}
                    />
                </LocalizationProvider>
                <TextField
                    label="Distance"
                    id="distance-input"
                    sx={{ m: 1 }}
                    value={distance}
                    onChange={(e) => setDistance(e.target.value)}
                    InputProps={{
                        endAdornment: <InputAdornment position="end">m</InputAdornment>,
                    }}
                />
                <Button variant='contained' sx={{ m: 1 }} onClick={handleClick}>Set</Button>
                <TextField
                    label="Speed Profile Name"
                    id="name-input"
                    sx={{ m: 1 }}
                    value={name}
                    onChange={(e) => setName(e.target.value)}
                />
                <Button variant='contained' sx={{ m: 1 }} onClick={handleSave}>Save</Button>
            </FormControl>
        </Box>
    );
}

export default SpeedProfiles;