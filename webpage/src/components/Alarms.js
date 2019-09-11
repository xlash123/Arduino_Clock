import React from 'react'
import { withStyles } from '@material-ui/core/styles';
import {
	Typography, ExpansionPanel, ExpansionPanelSummary,
	ExpansionPanelDetails, Button, Select,
	MenuItem,
} from '@material-ui/core'
import { ExpandMore } from '@material-ui/icons'
import axios from 'axios'

const hours = Array(12).fill(0).map((x, i) => i+1);
const minutes = Array(60).fill(0).map((x, i) => i);

console.log(hours);

class Alarms extends React.Component {

	constructor(props){
		super(props);

		this.state = {
			timeOpen: false, newAlarm: { hours: 1, minutes: 0, isAm: true }, 
		};
	}

	onChangeNewAlarm(data) {
		const newAlarm = { ...this.state.newAlarm, ...data };
		this.setState({ newAlarm })
	}

	render() {
		const { newAlarm } = this.state;

		return (
			<div>
				<Typography color='inherit' variant='h3'>
					Alarms
				</Typography>
				<ExpansionPanel>
					<ExpansionPanelSummary expandIcon={<ExpandMore />}>
						<Typography>Create New Alarm</Typography>
					</ExpansionPanelSummary>
					<ExpansionPanelDetails>
						<Typography>Enter time: </Typography>
						<Select
							value={newAlarm.hours}
							onChange={(e) => this.onChangeNewAlarm({hours: e.target.value})}
						>
							{hours.map(i => (
								<MenuItem key={`hours-select-${i}`} value={i}>
									{i}
								</MenuItem>	
							))};
						</Select>
						:
						<Select
							value={newAlarm.minutes}
							onChange={(e) => this.onChangeNewAlarm({minutes: e.target.value})}
						>
							{minutes.map(i => (
								<MenuItem key={`minutes-select-${i}`} value={i}>
									{i < 10 ? `0${i}` : i}
								</MenuItem>	
							))};
						</Select>
						<Select
							value={newAlarm.isAm ? 'AM' : 'PM'}
							onChange={(e) => this.onChangeNewAlarm({isAm: e.target.value === 'AM'})}
						>
							<MenuItem value="AM">AM</MenuItem>
							<MenuItem value="PM">PM</MenuItem>
						</Select>
					</ExpansionPanelDetails>
				</ExpansionPanel>
			</div>
		)
	}

}

export default Alarms