import React from 'react'
import { withStyles } from '@material-ui/core/styles';
import { Typography, ExpansionPanel, ExpansionPanelSummary, ExpansionPanelDetails } from '@material-ui/core'
import { ExpandMore } from '@material-ui/icons'

class Alarms extends React.Component {

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
						<Typography>Enter time:</Typography>
					</ExpansionPanelDetails>
				</ExpansionPanel>
			</div>
		)
	}

}

export default Alarms