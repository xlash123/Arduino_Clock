import React from 'react'
import classNames from 'classnames';
import { withStyles } from '@material-ui/core/styles';
import { CssBaseline, AppBar, IconButton, Toolbar, Typography } from '@material-ui/core'
import { Menu } from '@material-ui/icons'

import AppDrawer from './AppDrawer'

const drawerWidth = 240;

const styles = theme => ({
  root: {
    display: 'flex',
    flexWrap: 'wrap'
  },
  hide: {
    display: 'none',
  },
  menuButton: {
  },
  appBar: {
    transition: theme.transitions.create(['margin', 'width'], {
      easing: theme.transitions.easing.sharp,
      duration: theme.transitions.duration.leavingScreen,
    }),
  },
  appBarShift: {
    width: `calc(100% - ${drawerWidth}px)`,
    marginLeft: drawerWidth,
    transition: theme.transitions.create(['margin', 'width'], {
      easing: theme.transitions.easing.easeOut,
      duration: theme.transitions.duration.enteringScreen,
    }),
  },
  content: {
    flexGrow: 1,
    padding: theme.spacing.unit * 3,
    transition: theme.transitions.create('margin', {
      easing: theme.transitions.easing.sharp,
      duration: theme.transitions.duration.leavingScreen,
    }),
    marginLeft: -drawerWidth,
  },
  contentShift: {
    transition: theme.transitions.create('margin', {
      easing: theme.transitions.easing.easeOut,
      duration: theme.transitions.duration.enteringScreen,
    }),
    marginLeft: 0,
  },
  drawerHeader: {
    display: 'flex',
    alignItems: 'center',
    padding: '0 8px',
    ...theme.mixins.toolbar,
    justifyContent: 'flex-start',
  },
  drawer: {
    width: drawerWidth,
    flexShrink: 0,
  },
});

class App extends React.Component{

	constructor(props){
		super(props)

		this.state = {drawerOpen: false, currentPage: undefined}
	}

	render(){
		const {classes} = this.props;
		const {drawerOpen} = this.state;

    const Page = this.state.currentPage;

		return (
			<div className={classes.root}>
        <CssBaseline />
				<AppBar position='static' className={classNames(classes.appBar, {[classes.appBarShift]: drawerOpen})}>
					<Toolbar disableGutters={!drawerOpen}>
						<IconButton className={classNames(classes.menuButton, drawerOpen && classes.hide)} color="inherit" aria-label="Menu" onClick={() => this.setState({drawerOpen: true})}>
              <Menu />
            </IconButton>
            <Typography variant='h6' color='inherit' nowrap='true'>
            	Arduino Clock Webconfig
            </Typography>
					</Toolbar>
				</AppBar>
				<AppDrawer className={classes.drawer} open={drawerOpen} handleClose={() => this.setState({drawerOpen: false})} setComponent={(Component) => this.setState({currentPage: Component})} />
				<main className={classNames(classes.content, {[classes.contentShift]: drawerOpen})}>
          <div className={classes.drawerHeader}>
            {Page ? <Page /> : <Typography paragraph>No page here</Typography>}
          </div>
				</main>
			</div>
		);
	}

}

export default withStyles(styles, {withTheme: true})(App)