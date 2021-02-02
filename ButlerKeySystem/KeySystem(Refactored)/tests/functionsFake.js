const QueryList =require('./quariesFake');


exports.simple = async () => {
    let finalResults = QueryList.simpleTest().then( results => {
        console.log(results);
        return results
    });
    return finalResults;
}