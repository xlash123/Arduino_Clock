import React from 'react'
import { withStyles } from '@material-ui/core/styles';
import { Drawer, List, ListItem, ListItemText, IconButton, Divider } from '@material-ui/core'
import { ChevronLeft } from '@material-ui/icons';

import Alarms from './Alarms'

const drawerWidth = 240;

const styles = theme => ({
  drawer: {
    width: drawerWidth,
    flexShrink: 0,
  },
  drawerPaper: {
    width: drawerWidth,
  },
  drawerHeader: {
    display: 'flex',
    alignItems: 'center',
    padding: '0 8px',
    ...theme.mixins.toolbar,
    justifyContent: 'flex-end',
  },
});

class AppDrawer extends React.Component{

	render(){
		const { classes, theme } = this.props;

    	return (
    		<Drawer
    			className={classes.drawer}
    			variant='persistent'
    			anchor='left'
    			open={this.props.open}
    			classes={{paper: classes.drawerPaper}}
    		>
    			<div className={classes.drawerHeader}>
    				<IconButton onClick={this.props.handleClose}>
    					<ChevronLeft />
    				</IconButton>
    			</div>
    			<List>
    				<ListItem button key='Alarms' onClick={() => this.props.setComponent(Alarms)}>
    					<ListItemText primary='Alarms' />
    				</ListItem>
                    <ListItem button key='Notes'>
                      <ListItemText primary='Notes' />
                    </ListItem>
    			</List>
                <Divider />
    		</Drawer>
		)
	}

}

export default withStyles(styles, {withTheme: true})(AppDrawer)