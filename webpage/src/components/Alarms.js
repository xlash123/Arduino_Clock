import React from 'react'
import { withStyles } from '@material-ui/core/styles';
import { Typography, ExpansionPanel, ExpansionPanelSummary, ExpansionPanelDetails, Button, Select } from '@material-ui/core'
import { ExpandMore } from '@material-ui/icons'
import axios from 'axios'

class Alarms extends React.Component {

	component(props){
		super(props)

		this.state = {'timeOpen': false};
	}

	render(){
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
						<Select >
						</Select>
					</ExpansionPanelDetails>
				</ExpansionPanel>
			</div>
		)
	}

}

export default Alarms