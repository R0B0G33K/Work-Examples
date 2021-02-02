//makes sure it is not past closing tim
function TimeUpMiddleware () {  
    var todayHours = new Date().getHours();
    var todayMin = new Date().getMinutes();
	return (req, res, next) => {
        if (todayHours < 16 || (todayHours == 16 && todayMin <= 30)) return next();
	    res.redirect('/inpcheckout')
	}
}

//makes sure one is logged in
function authenticationMiddleware () {  
	return (req, res, next) => {
	    if (req.isAuthenticated()) return next();
	    res.redirect('/')
	}
}

//gets all dates between two
const getDatesBetweenDates = (startDate, endDate) => {
    let dates = []
    //to avoid modifying the original date
    const theDate = new Date(startDate)
    const theEDate = new Date(endDate)
    console.log(theDate);
    while (theDate < theEDate) {
      dates = [...dates, new Date(theDate)]
      theDate.setDate(theDate.getDate() + 1)
    }
    dates = [...dates, theEDate]
    return dates
}


modules.exports = TimeUpMiddleware;
modules.exports = authenticationMiddleware;
modules.exports = getDatesBetweenDates;