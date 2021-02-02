const { simple } = require('./functionsFake');

test('should output Apples array', async () =>{
    const array = await simple();
    answer = [1,2,3];
    expect(array).toEqual(answer);
});