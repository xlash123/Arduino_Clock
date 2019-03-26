import React from 'react'
import { withStyles } from '@material-ui/core/styles';
import { Drawer, List, ListItem, ListItemText, IconButton, Divider } from '@material-ui/core'
import { ChevronLeft } from '@material-ui/icons';

const drawerWidth = 240;

const styles = theme => ({
  root: {
    display: 'flex',
  },
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
    justifyContent: 'flex-end',
  }
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
    				<ListItem button key='test'>
    					<ListItemText primary='test' />
    				</ListItem>
    			</List>
    			<Divider />
    		</Drawer>
		)
	}

}

export default withStyles(styles)(AppDrawer)