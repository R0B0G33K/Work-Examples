'use strict';

module.exports = {
  up: async (queryInterface, Sequelize) => {
    return queryInterface.createTable(
      'persons',{
          personID: {
              type: Sequelize.INTEGER,
              autoIncrement: true,
              primaryKey: true
          },
          name:{
              type: Sequelize.STRING,
              allowNull: false
          },
          phoneNum:{
              type: Sequelize.STRING,
              primaryKey: true
          },
          company:{
              type: Sequelize.STRING
          },
          sponsName:{
              type: Sequelize.STRING
          },
          sponsEmail:{
              type: Sequelize.STRING
          },
          sponsPhoneNum:{
              type: Sequelize.STRING
          },
          signiture:{
              type: Sequelize.BLOB
          }
      })
  },

  down: async (queryInterface, Sequelize) => {
    await queryInterface.dropTable('persons');
  }
};
