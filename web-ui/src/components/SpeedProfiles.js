import React, { useState } from 'react';
import Box from '@mui/material/Box';
import TextField from '@mui/material/TextField';
import InputAdornment from '@mui/material/InputAdornment';
import FormControl from '@mui/material/FormControl';
import Select from '@mui/material/Select';
import InputLabel from '@mui/material/InputLabel';
import MenuItem from '@mui/material/MenuItem';
import Button from '@mui/material/Button';
import { useMemo } from 'react';
import speedProfilesData from '../data/speed_profiles.json';

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
    const [time, setTime] = useState('');
    const [distance, setDistance] = useState('');
    const [selectedProfile, setSelectedProfile] = useState("");
    const [name, setName] = useState("");

    function handleSelect(event) {
        const select = event.target.value;
        setSelectedProfile(select);

        if (select !== 'custom') {
            setTime(speedProfilesData[select].time);
            setDistance(speedProfilesData[select].distance);
        }
        else {
            setTime('');
            setDistance('');
        }
    }

    // submit inputs and send to robot
    function handleClick() {
        const data = {
            time: time,
            distance: distance
        }

        console.log('submit inputs:', data);

        fetch('/postData', { // ZAK: CHANGE PATH HERE
            method: 'POST',
            body: JSON.stringify(data)
        })
    }

    // save speed profile to json /data/speed_profiles.json
    function handleSave() {
        const data = {
            name: name,
            time: time,
            distance: distance
        }

        fetch('/addSpeedProfile', {
            method: 'POST',
            body: JSON.stringify(data)
        })
    }

    return (
        <Box
            component="form"
            autoComplete="off"
            sx={{ display: 'flex', flexDirection: 'column', width: 500, maxWidth: '100%' }}
        >
            {/* eventually this will send data to the robot */}
            <FormControl fullWidth >
                <InputLabel id="simple-select-helper-label" sx={{ m: 1 }}>Speed Profile</InputLabel>
                <Select
                    fullWidth
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

                <TextField
                    fullWidth
                    label="Time"
                    id="time-input"
                    sx={{ m: 1 }}
                    value={time}
                    onChange={(e) => setTime(e.target.value)}
                    InputProps={{
                        endAdornment: <InputAdornment position="end">s</InputAdornment>,
                    }}
                />
                <TextField
                    fullWidth
                    label="Distance"
                    id="distance-input"
                    sx={{ m: 1 }}
                    value={distance}
                    onChange={(e) => setDistance(e.target.value)}
                    InputProps={{
                        endAdornment: <InputAdornment position="end">m</InputAdornment>,
                    }}
                />
                <Button variant='contained' sx={{ m: 1 }} fullWidth onClick={handleClick}>Set</Button>
                <TextField
                    fullWidth
                    label="Speed Profile Name"
                    id="name-input"
                    sx={{ m: 1 }}
                    value={name}
                    onChange={(e) => setName(e.target.value)}
                />
                <Button variant='contained' sx={{ m: 1 }} fullWidth onClick={handleSave}>Save</Button>
            </FormControl>
        </Box>
    );
}

export default SpeedProfiles;